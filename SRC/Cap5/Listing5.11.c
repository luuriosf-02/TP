#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>   // para htonl

/* Escribir texto en el socket */
void write_text(int socket_fd, const char* text) {
    int length = strlen(text) + 1;
    int netlen = htonl(length);  // conversión host → network

    if (write(socket_fd, &netlen, sizeof(netlen)) == -1) {
        perror("write length");
        exit(EXIT_FAILURE);
    }
    if (write(socket_fd, text, length) == -1) {
        perror("write text");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* const argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <socket_name> <mensaje>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* const socket_name = argv[1];
    const char* const message = argv[2];

    int socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un name;
    memset(&name, 0, sizeof(struct sockaddr_un));
    name.sun_family = AF_LOCAL;

    if (strlen(socket_name) >= sizeof(name.sun_path)) {
        fprintf(stderr, "Error: socket_name demasiado largo\n");
        exit(EXIT_FAILURE);
    }
    strcpy(name.sun_path, socket_name);

    if (connect(socket_fd, (struct sockaddr *)&name, SUN_LEN(&name)) == -1) {
        perror("connect");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    write_text(socket_fd, message);

    close(socket_fd);
    return 0;
}

