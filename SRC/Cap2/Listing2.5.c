

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/* A handle for a temporary file created with write_temp_file. In
   this implementation, it's just a file descriptor. */
typedef int temp_file_handle;

/* Writes LENGTH bytes from BUFFER into a temporary file. The
   temporary file is immediately unlinked. Returns a handle to the
   temporary file. */
temp_file_handle write_temp_file(char* buffer, size_t length)
{
    char temp_filename[] = "/tmp/temp_file.XXXXXX";
    int fd = mkstemp(temp_filename);

    if (fd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }

    unlink(temp_filename); // Eliminar el archivo inmediatamente

    if (write(fd, &length, sizeof(length)) != sizeof(length)) {
        perror("write length");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (write(fd, buffer, length) != (ssize_t)length) {
        perror("write buffer");
        close(fd);
        exit(EXIT_FAILURE);
    }

    return fd;
}

/* Reads the contents of a temporary file TEMP_FILE created with
   write_temp_file. The return value is a newly allocated buffer of
   those contents, which the caller must deallocate with free.
   *LENGTH is set to the size of the contents, in bytes. The
   temporary file is removed. */
char* read_temp_file(temp_file_handle temp_file, size_t* length)
{
    char* buffer;
    int fd = temp_file;

    lseek(fd, 0, SEEK_SET);

    if (read(fd, length, sizeof(*length)) != sizeof(*length)) {
        perror("read length");
        close(fd);
        exit(EXIT_FAILURE);
    }

    buffer = (char*)malloc(*length);
    if (!buffer) {
        perror("malloc");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (read(fd, buffer, *length) != (ssize_t)(*length)) {
        perror("read buffer");
        free(buffer);
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd); // Eliminar temporal al cerrar
    return buffer;
}

/* Ejemplo de uso de las funciones */
int main() {
    char data[] = "Hola, mundo!";
    size_t length;
    
    // Escribir datos en archivo temporal
    temp_file_handle fd = write_temp_file(data, strlen(data));

    // Leer los datos del archivo temporal
    char* read_data = read_temp_file(fd, &length);

    // Mostrar resultados
    printf("Datos leídos: %.*s\n", (int)length, read_data);

    // Liberar memoria
    free(read_data);

    return 0;
}

