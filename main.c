#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>

#include "server.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Error en los argumentos. Uso: ./program <puerto> <directorio>\n");
        return 1;
    }

    int port = atoi(argv[1]);
    char *directory = argv[2];
    
    signal(SIGINT, sigint_handler);

    struct stat st;
    if (stat(directory, &st) == 0 && S_ISDIR(st.st_mode)) {
        start_server(port, directory);
    }
    else {
        printf("El directorio especificado no existe\n");
        return 1;
    }

    return 0;
}