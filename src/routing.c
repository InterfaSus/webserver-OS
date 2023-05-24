#include <stdio.h>
#include <sys/stat.h>

#include "constants.h"
#include "page.h"

void router(char *main_directory, char *relative, char response[]) {

    char full_dir[MAX_FILE_ROUTE];
    sprintf(full_dir, "%s%s", main_directory, relative);

    // Revisar si el directorio existe
    struct stat st;
    if (stat(full_dir, &st) == -1) {
        printf("Error al obtener los atributos del archivo %s\n", full_dir);
        return;
    }

    // Revisar si es un archivo
    if (!S_ISDIR(st.st_mode)) {
        printf("Es un archivo!. Descargar\n");
    }
    else { // Es una carpeta
        generate_page(main_directory, relative, response);
    }
}