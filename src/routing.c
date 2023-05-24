#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "page.h"
#include "string_helpers.h"

void router(char *main_directory, char *relative, char response[]) {

    char full_dir[MAX_FILE_ROUTE];
    sprintf(full_dir, "%s%s", main_directory, relative);
    urlDecode(full_dir);

    // Revisar si el directorio existe
    struct stat st;
    if (stat(full_dir, &st) == -1) {
        printf("Error al obtener los atributos del archivo %s\n", full_dir);
        // Enviar error 404
        return;
    }

    // Revisar si es un archivo
    if (!S_ISDIR(st.st_mode)) {
        printf("Es un archivo!. Descargar\n");
    }
    else { // Es una carpeta
        generate_page(full_dir, relative, response);
    }
}