#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>

#include "constants.h"
#include "string_helpers.h"

void generate_page(char *directory, char response[]) {

    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    char body[MAX_PAGE_SIZE];

        sprintf(body,
        "<!DOCTYPE html>"
        "<html>"
            "<head>"
                "<title>Directorio %s</title>"
                "<meta charset=\"UTF-8\">"
            "</head>"
            "<body>"
                "<table>"
                    "<tr>"
                        "<th>Nombre</th>"
                        "<th>Tamaño</th>"
                        "<th>Fecha de Modificación</th>",
        directory);
        

    char closure[] = "<tr>"
                "<table>"
            "</body>"
        "</html>";

    DIR *dir;
    struct dirent *ent;
    char *archivos[MAX_FILES];
    time_t fechas[MAX_FILES];
    int tamanos[MAX_FILES];
    int num_archivos = 0;

    // Abrir el directorio
    dir = opendir(directory);
    if (dir == NULL) {
        printf("No se pudo abrir el directorio.\n");
        return;
    }

    // Leer los archivos y carpetas dentro del directorio
    while ((ent = readdir(dir)) != NULL) {
        // Ignorar los archivos "." y ".."
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        // Almacenar el nombre del archivo en la lista
        archivos[num_archivos] = strdup(ent->d_name);
        
        // Obtener la fecha de modificación y el tamaño del archivo
        char ruta[512];
        snprintf(ruta, sizeof(ruta), "%s/%s", directory, ent->d_name);
        struct stat atributos;
        if (stat(ruta, &atributos) == 0) {
            // Si el elemento no es una carpeta, almacenar su tamaño
            if (!S_ISDIR(atributos.st_mode)) {
                tamanos[num_archivos] = atributos.st_size;
            } else {
                tamanos[num_archivos] = -1;
            }
            fechas[num_archivos] = atributos.st_mtime;
        } else {
            fechas[num_archivos] = 0;
            tamanos[num_archivos] = 0;
        }

        num_archivos++;

        // Verificar si se ha alcanzado el límite máximo
        if (num_archivos >= MAX_FILES) {
            printf("Se ha alcanzado el límite máximo de archivos.\n");
            break;
        }
    }

    // Cerrar el directorio
    closedir(dir);

    // Agregar los nombres de archivos a la página
    for (int i = 0; i < num_archivos; i++) {
        
        char time_buff[20];
        strftime(time_buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&fechas[i]));
        char size_buff[5];
        bytes_to_string(tamanos[i], size_buff);

        char archivo[MAX_ROW_SIZE];

        sprintf(archivo,
            "<tr>"
                "<td>%s</td>"
                "<td>%s</td>"
                "<td>%s</td>"
            "</tr>",
            archivos[i], size_buff, time_buff);
        
        strcat(body, archivo);
    }

    strcat(response, closure);

    // Liberar la memoria asignada a los nombres de archivos en la lista
    for (int i = 0; i < num_archivos; i++) {
        free(archivos[i]);
    }

    strcpy(response, header);
    strcat(response, body);
}