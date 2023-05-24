#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void bytes_to_string(int file_size, char buffer[]) {

    if (file_size < 0) {
        sprintf(buffer, "-");
        return;
    }

    float val = file_size;
    int order = 0;

    while(val > 1000) {

        val /= 1000;
        order++;
    }
    
    char suffix = '\0';

    if (order == 1) suffix = 'K';
    else if (order == 2) suffix = 'M';
    else if (order == 3) suffix = 'G';
    else if (order == 4) suffix = 'T';

    char size_buf[4];
    gcvt(val, val < 100 ? 2 : 3, size_buf);

    sprintf(buffer, "%s%c", size_buf, suffix);
}

char hexToChar(char *hex) {
    char c;
    sscanf(hex, "%2hhx", &c);
    return c;
}

void urlDecode(char *url) {
    int len = strlen(url);
    char *decoded = (char *)malloc(len + 1);
    int i, j = 0;
    
    for (i = 0; i < len; i++) {
        if (url[i] == '%' && i + 2 < len) {
            decoded[j++] = hexToChar(&url[i + 1]);
            i += 2;
        } else if (url[i] == '+') {
            decoded[j++] = ' ';
        } else {
            decoded[j++] = url[i];
        }
    }
    
    decoded[j] = '\0';
    
    strcpy(url, decoded);
    
    free(decoded);
}