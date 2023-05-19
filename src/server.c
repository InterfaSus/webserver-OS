#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int default_port = 8080;

int start_server(int port, char *directory) {

    default_port = port;

    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hola desde el servidor web!</h1>";

    // Crear un socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al abrir el socket");
        exit(1);
    }

    // Configurar la estructura de la dirección del servidor
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Vincular el socket al puerto
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error al vincular");
        exit(1);
    }

    // Escuchar en el socket
    listen(sockfd, 5);
    printf("Servidor escuchando en el puerto %d...\n", port);

    while (1) {
        clilen = sizeof(cli_addr);

        // Aceptar la conexión entrante
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error al aceptar la conexión");
            exit(1);
        }

        // Enviar la respuesta básica al cliente
        write(newsockfd, response, sizeof(response) - 1);

        // Cerrar la conexión entrante
        close(newsockfd);
    }

    // Cerrar el socket principal
    close(sockfd);
}