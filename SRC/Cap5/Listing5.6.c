#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FILE_LENGTH 0x100  /* 256 bytes */

int main(int argc, char* const argv[]) {
    int fd;
    void* file_memory;
    int integer;
    char *buf;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    /* Abrir el archivo (debe existir y tener al menos FILE_LENGTH bytes) */
    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    /* Mapear el archivo en memoria con lectura y escritura */
    file_memory = mmap(NULL, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    /* Asegurarse de que el contenido sea una cadena NUL-terminated antes de usar sscanf/atoi.
       Aquí copiamos a un buffer local para añadir NUL seguro. */
    buf = malloc(FILE_LENGTH + 1);
    if (!buf) {
        perror("malloc");
        munmap(file_memory, FILE_LENGTH);
        return 1;
    }
    memcpy(buf, file_memory, FILE_LENGTH);
    buf[FILE_LENGTH] = '\0'; /* seguridad: terminar la cadena */

    /* Leer el entero desde la cadena (sscanf lee desde la cadena) */
    if (sscanf(buf, "%d", &integer) != 1) {
        fprintf(stderr, "No se pudo parsear un entero en el archivo.\n");
        free(buf);
        munmap(file_memory, FILE_LENGTH);
        return 1;
    }
    printf("value: %d\n", integer);

    /* Escribir el doble en la memoria mapeada (formateado como texto) */
    snprintf(buf, FILE_LENGTH, "%d\n", 2 * integer);
    /* Copiamos de vuelta al area mapeada y opcionalmente forzamos escritura al disco */
    memcpy(file_memory, buf, strlen(buf) + 1);
    if (msync(file_memory, FILE_LENGTH, MS_SYNC) == -1) {
        perror("msync");
        /* no fatal: seguimos con la limpieza */
    }

    free(buf);
    munmap(file_memory, FILE_LENGTH);
    return 0;
}

