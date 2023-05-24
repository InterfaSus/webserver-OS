#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "routing.h"
#include "constants.h"
#include "string_helpers.h"

char *main_directory;
int sockfd;

void sigint_handler(int sig) {

    printf("\n\nCerrando el servidor...\n");
    close(sockfd);
    exit(0);
}

int start_server(int port, char *directory) {

    main_directory = directory;

    int newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

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

    // Obtener la dirección IP del servidor
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        perror("Error al obtener la dirección IP de la interfaz de red");
        exit(1);
    }

    char local_ip[INET_ADDRSTRLEN];
    memset(local_ip, 0, sizeof(local_ip));

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), local_ip, INET_ADDRSTRLEN);
            if (strcmp(ifa->ifa_name, "lo") != 0) {
                break;  // Se encontró una interfaz de red diferente a "lo" (loopback)
            }
        }
    }

    freeifaddrs(ifaddr);

    // Escuchar en el socket
    listen(sockfd, 5);
    printf("\nServidor escuchando en el puerto %d...\n\n", port);
    printf("Conectarse localmente: http://localhost:%d\n", port);
    printf("Red local: http://%s:%d\n\n", local_ip, port);

    while (1) {
        clilen = sizeof(cli_addr);

        // Aceptar la conexión entrante
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error al aceptar la conexión");
            exit(1);
        }

        // Leer la solicitud del cliente
        char request[MAX_REQUEST_SIZE];
        memset(request, 0, sizeof(request));
        ssize_t bytesRead = read(newsockfd, request, sizeof(request)-1);
        if (bytesRead < 0) {
            perror("Error al leer la solicitud del cliente");
            exit(1);
        }

        // Imprimir la solicitud del cliente
        printf("Solicitud del cliente:\n\n%s\n", request);

        char *requestLine = strtok(request, "\r\n"); // Obtener la primera línea de la petición
        char *method = strtok(requestLine, " "); // Obtener el método HTTP (GET, POST, etc.)
        char *path = strtok(NULL, " "); // Obtener la dirección de la petición

        char response[MAX_PAGE_SIZE];
        FILE *file;
        int isDownload = router(main_directory, path, response);

        // Enviar la respuesta básica al cliente
        write(newsockfd, response, strlen(response));

        // Enviar el archivo al cliente
        if (isDownload == 1) {

            char full_dir[MAX_FILE_ROUTE];
            sprintf(full_dir, "%s%s", main_directory, path);
            urlDecode(full_dir);
            FILE *file = fopen(full_dir, "rb");

            char buffer[DOWNLOAD_CHUNK_SIZE];
            memset(buffer, 0, sizeof(buffer));
            if (file == NULL) {
                perror("Error al abrir el archivo");
                exit(1);
            }
            while (1) {
                size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
                if (bytesRead <= 0) {
                    break;
                }

                write(newsockfd, buffer, bytesRead);
            }

            fclose(file);
        }

        // Cerrar la conexión entrante
        close(newsockfd);
    }

    // Cerrar el socket principal
    close(sockfd);
}
