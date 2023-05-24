#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "server.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Error en los argumentos. Uso: ./program <puerto> <directorio>\n");
        return 1;
    }

    int port = atoi(argv[1]);
    char *directory = argv[2];
    
    signal(SIGINT, sigint_handler);

    start_server(port, directory);

    return 0;
}