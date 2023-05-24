#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "page.h"
#include "string_helpers.h"

void send404(char response[]) {

    sprintf(response, "HTTP/1.1 404 Not Found\r\n");
    sprintf(response + strlen(response), "Content-Type: text/html\r\n");
    sprintf(response + strlen(response), "\r\n");
    sprintf(response + strlen(response), "<h1>404 Not Found</h1>");
}

int router(char *main_directory, char *relative, char response[]) {

    char full_dir[MAX_FILE_ROUTE];
    sprintf(full_dir, "%s%s", main_directory, relative);
    urlDecode(full_dir);

    // Revisar si el directorio existe
    struct stat st;
    if (stat(full_dir, &st) == -1) {
        printf("Error al obtener los atributos del archivo %s\n", full_dir);
        
        // Enviar error 404
        send404(response);

        return 0;
    }

    // Revisar si es un archivo
    if (!S_ISDIR(st.st_mode)) {
        
        FILE *file = fopen(full_dir, "rb");
        if (file == NULL) {
            printf("Error al abrir el archivo %s\n", full_dir);
            
            // Enviar error 404
            send404(response);

            return 0;
        }

        // Obtener el tamaño del archivo
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Crear las cabeceras HTTP
        sprintf(response, "HTTP/1.1 200 OK\r\n");
        sprintf(response + strlen(response), "Content-Disposition: attachment; filename=\"%s\"\r\n", full_dir);
        sprintf(response + strlen(response), "Content-Type: application/octet-stream\r\n");
        sprintf(response + strlen(response), "Content-Length: %ld\r\n", fileSize);
        sprintf(response + strlen(response), "\r\n");

        fclose(file);

        return 1;
    }
    else { // Es una carpeta
        generate_page(full_dir, relative, response);
    }

    return 0;
}