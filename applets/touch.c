/* 
** touch.c
**
** A custom touch by x4x
** 20250321 x4x
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "applets.h"

int touch_main(int argc, char **argv)
{
    int fd;
    
    if (argc < 2) {
        printf("usage: %s <file>...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        fd = open(argv[i], O_CREAT | O_WRONLY, 0644);
        if ( fd == -1) {
            fprintf(stderr, "Failed to create file: %s\n", argv[i]);
            return 1;
        }
        close(fd);
    }
    return 0;
}
