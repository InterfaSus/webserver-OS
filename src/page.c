#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>

#include "constants.h"
#include "string_helpers.h"

int compare(const struct dirent **a, const struct dirent **b) {
    return strcoll((*a)->d_name, (*b)->d_name);
}


void generate_page(char *directory, char *relative, char response[]) {

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
                "<h1>Index of %s</h1>"
                "<table>"
                    "<tr>"
                        "<th>Nombre</th>"
                        "<th>Tamaño</th>"
                        "<th>Fecha de Modificación</th>",
        directory, relative);
        

    char closure[] = "<tr>"
                "<table>"
            "</body>"
        "</html>";

    struct dirent **namelist;
    int num_entries;

    char *file_names[MAX_FILES];
    time_t file_dates[MAX_FILES];
    int file_sizes[MAX_FILES];
    int n_files = 0;

    char *folder_names[MAX_FILES];
    time_t folder_dates[MAX_FILES];
    int n_folders = 0;

    // Abrir el directorio
    num_entries = scandir(directory, &namelist, NULL, compare);
    if (num_entries == -1) {
        printf("No se pudo abrir el directorio.\n");
        return;
    }

    // Leer los archivos y carpetas dentro del directorio en orden alfabético
    for (int i = 0; i < num_entries; i++) {
        struct dirent *ent = namelist[i];

        // Ignorar los archivos "." y ".."
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        
        // Obtener la fecha de modificación y el tamaño del archivo
        char ruta[MAX_FILE_ROUTE];
        snprintf(ruta, sizeof(ruta), "%s/%s", directory, ent->d_name);
        struct stat atributos;
        if (stat(ruta, &atributos) == 0) {
            // Si el elemento no es una carpeta, almacenar su tamaño
            if (!S_ISDIR(atributos.st_mode)) {

                file_names[n_files] = strdup(ent->d_name);
                file_sizes[n_files] = atributos.st_size;
                file_dates[n_files] = atributos.st_mtime;
                n_files++;
            }
            else {

                folder_names[n_folders] = strdup(ent->d_name);
                folder_dates[n_folders] = atributos.st_mtime;
                n_folders++;
            }
        }

        free(ent);
    }
    free(namelist);
    
    // Agregar los nombres de archivos a la página
    for (int i = 0; i < n_files; i++) {
        
        char time_buff[20];
        strftime(time_buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&file_dates[i]));
        char size_buff[5];
        bytes_to_string(file_sizes[i], size_buff);

        char element[MAX_ROW_SIZE];
        char item_route[MAX_FILE_ROUTE];

        sprintf(item_route, "%s/%s", relative, file_names[i]);

        sprintf(element,
            "<tr>"
                "<td><a href=\"%s\">%s</a></td>"
                "<td>%s</td>"
                "<td>%s</td>"
            "</tr>",
            item_route, file_names[i], size_buff, time_buff);
        
        strcat(body, element);
    }

    // Agregar los nombres de carpetas a la página
    for (int i = 0; i < n_folders; i++) {
        
        char time_buff[20];
        strftime(time_buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&folder_dates[i]));

        char element[MAX_ROW_SIZE];
        char item_route[MAX_FILE_ROUTE];

        sprintf(item_route, "%s/%s", relative, folder_names[i]);

        sprintf(element,
            "<tr>"
                "<td><a href=\"%s\">%s</a></td>"
                "<td>-</td>"
                "<td>%s</td>"
            "</tr>",
            item_route, folder_names[i], time_buff);
        
        strcat(body, element);
    }


    // Liberar la memoria asignada a los nombres de archivos en la lista
    for (int i = 0; i < n_files; i++) {
        free(file_names[i]);
    }
    // Liberar la memoria asignada a los nombres de carpetas en la lista
    for (int i = 0; i < n_folders; i++) {
        free(folder_names[i]);
    }

    strcpy(response, header);
    strcat(response, body);
    strcat(response, closure);
}