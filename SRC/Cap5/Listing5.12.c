#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

/* Descargar la página principal */
void get_home_page(int socket_fd, const char *hostname) {
    char buffer[10000];
    ssize_t number_characters_read;

    /* Enviar petición HTTP con Host dinámico */
    snprintf(buffer, sizeof(buffer),
             "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", hostname);
    write(socket_fd, buffer, strlen(buffer));

    while (1) {
        number_characters_read = read(socket_fd, buffer, sizeof(buffer));
        if (number_characters_read == 0)
            return;
        fwrite(buffer, sizeof(char), number_characters_read, stdout);
    }
}


int main(int argc, char* const argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <hostname>\n", argv[0]);
        return 1;
    }

    int socket_fd;
    struct sockaddr_in name;
    struct hostent* hostinfo;

    /* Crear socket */
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        return 1;
    }

    /* Resolver hostname */
    hostinfo = gethostbyname(argv[1]);
    if (hostinfo == NULL) {
        fprintf(stderr, "Host desconocido: %s\n", argv[1]);
        return 1;
    }

    /* Preparar sockaddr_in */
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
    name.sin_port = htons(80);

    /* Conectar */
    if (connect(socket_fd, (struct sockaddr *)&name, sizeof(name)) == -1) {
        perror("connect");
        return 1;
    }

    /* Descargar página */
    get_home_page(socket_fd, argv[1]);


    close(socket_fd);
    return 0;
}

