

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

typedef int temp_file_handle;


temp_file_handle write_temp_file(char* buffer, size_t length)
{
    char temp_filename[] = "/tmp/temp_file.XXXXXX";
    int fd = mkstemp(temp_filename);

    if (fd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }

    unlink(temp_filename); 

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

    close(fd); 
    return buffer;
}


int main() {
    char data[] = "Hola, mundo, este es el TP de MAra y Lujan :)!";
    size_t length;
    

    temp_file_handle fd = write_temp_file(data, strlen(data));

 
    char* read_data = read_temp_file(fd, &length);


    printf("Los datos leidos son: %.*s\n", (int)length, read_data);

    free(read_data);

    return 0;
}

