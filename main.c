#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Error en los argumentos. Uso: ./program <puerto> <directorio>\n");
        return 1;
    }

    int port = atoi(argv[1]);
    char *directory = argv[2];
    
    printf("Puerto: %d\n", port);
    printf("Directorio: %s\n", directory);

    return 0;
}