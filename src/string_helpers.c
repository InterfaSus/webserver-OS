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
