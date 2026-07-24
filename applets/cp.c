/* 
** cp.c
**
** A custom cp by x4x
** 20250318 x4x
*/

#include <stdio.h>
#include <string.h>

#define BUFSIZE 4096


int cp_main(int argc, char **argv)
{
    //check if enough arguments
    if (argc < 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if (src == NULL) {
        printf("Unable to open source file %s\n", argv[1]);
        return 1;
    }

    FILE *dst = fopen(argv[2], "w");
    if (dst == NULL) {
        printf("Unable to open destination file %s\n", argv[2]);
        fclose(src);
        return 1;
    }

    char buf[BUFSIZE];
    size_t n;

    while((n = fread(buf, 1, sizeof(buf), src)) > 0 ) {
        if (fwrite(buf, 1, n, dst) != n) {
            printf("Error");
            fclose(src);
            fclose(dst);
            return 1;
        }
    }

    if (ferror(src)) {
        printf("Error while reading\n");
        fclose(src);
        fclose(dst);
        return 1;
    }

    fclose(src);
    fclose(dst);
    return 0;
}
