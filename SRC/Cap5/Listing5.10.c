#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <unistd.h>  
#include <arpa/inet.h>
/* Read text from the socket and print it out. Continue until the  
   socket closes. Return nonzero if the client sent a "quit"  
   message, zero otherwise.  */  
int server(int client_socket) {
    while (1) {
        int length;
        char* text;
        int n = 0;

        /* Leer longitud completa */
        while (n < sizeof(length)) {
            int r = read(client_socket, ((char*)&length)+n, sizeof(length)-n);
            if (r <= 0) return 0;  // cliente cerró
            n += r;
        }

        /* Convertir de red a host (seguro si cliente envía htonl) */
        length = ntohl(length);

        if (length <= 0 || length > 1024) {
            fprintf(stderr, "Longitud inválida: %d\n", length);
            return 0;
        }

        /* Leer mensaje completo */
        text = malloc(length + 1);
        if (!text) return 0;

        n = 0;
        while (n < length) {
            int r = read(client_socket, text+n, length-n);
            if (r <= 0) { free(text); return 0; }
            n += r;
        }

        text[length] = '\0';  // terminar cadena

        printf("%s\n", text);

        if (!strcmp(text, "quit")) { free(text); return 1; }

        free(text);
    }
}

int main(int argc, char* const argv[])  
{ 
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <socket_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* const socket_name = argv[1];  
    int socket_fd;  
    struct sockaddr_un name;  
    int client_sent_quit_message;  

    /* Crear socket */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);  
    if (socket_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Configurar sockaddr_un */
    memset(&name, 0, sizeof(struct sockaddr_un));
    name.sun_family = AF_LOCAL;

    if (strlen(socket_name) >= sizeof(name.sun_path)) {
        fprintf(stderr, "Error: socket_name demasiado largo\n");
        exit(EXIT_FAILURE);
    }
    strcpy(name.sun_path, socket_name);  

    /* Borrar si ya existe */
    unlink(socket_name);

    /* Asociar */
    if (bind(socket_fd, (struct sockaddr *)&name, SUN_LEN(&name)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Loop principal */
    do { 
        struct sockaddr_un client_name;  
        socklen_t client_name_len = sizeof(client_name);
        int client_socket_fd;  

        client_socket_fd = accept(socket_fd, (struct sockaddr *)&client_name, &client_name_len);
        if (client_socket_fd == -1) {
            perror("accept");
            continue;
        }
  
        client_sent_quit_message = server(client_socket_fd);  
        close(client_socket_fd);  
    } while (!client_sent_quit_message);  

    close(socket_fd);  
    unlink(socket_name);  
    return 0;  
}

