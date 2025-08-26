#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Función que ya tienes */
char* read_from_file(const char* filename, size_t length)
{
    char* buffer;
    int fd;
    ssize_t bytes_read;

    buffer = (char*) malloc(length);
    if (buffer == NULL)
        return NULL;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        free(buffer);
        return NULL;
    }

    bytes_read = read(fd, buffer, length);
    if (bytes_read != (ssize_t)length) {
        free(buffer);
        close(fd);
        return NULL;
    }

    close(fd);
    return buffer;
}

int main() {
    const char* filename = "test.txt";

    /* Obtener el tamaño del archivo */
    size_t length;
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fclose(f);

    /* Leer el contenido */
    char* data = read_from_file(filename, length);
    if (!data) {
        printf("Error al leer el archivo.\n");
        return 1;
    }

    /* Mostrar contenido */
    printf("Contenido: %.*s\n", (int)length, data);

    free(data);
    return 0;
}
