#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100  // 256 bytes

/* Número aleatorio uniforme en [low, high] */
int random_range(unsigned const low, unsigned const high) {
    unsigned const range = high - low + 1;
    return low + (int) (((double) range) * rand() / (RAND_MAX + 1.0));
}

int main(int argc, char* const argv[]) {
    int fd;
    void* file_memory;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    /* Semilla para números aleatorios */
    srand(time(NULL));

    /* Crear/abrir archivo */
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error al abrir archivo");
        return 1;
    }

    /* Ajustar tamaño del archivo */
    if (lseek(fd, FILE_LENGTH - 1, SEEK_SET) == -1) {
        perror("Error en lseek");
        close(fd);
        return 1;
    }
    if (write(fd, "", 1) != 1) {
        perror("Error en write");
        close(fd);
        return 1;
    }

    /* Mapear el archivo en memoria con lectura y escritura */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        perror("Error en mmap");
        close(fd);
        return 1;
    }
    close(fd);

    /* Escribir número aleatorio */
    sprintf((char*) file_memory, "%d\n", random_range(-100, 100));
    printf("Número escrito en memoria-mapeada: %s", (char*)file_memory);

    /* Liberar memoria */
    munmap(file_memory, FILE_LENGTH);

    return 0;
}

