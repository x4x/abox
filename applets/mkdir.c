/*
** mkdir.c
**
** A custom mkdir by x4x
** 20250321 x4x
*/

#include <stdio.h>
#include <sys/stat.h>

int mkdir_main(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s <directory>...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (mkdir(argv[i], 0755) == -1) {
            printf("Failed to create directory: %s\n", argv[i]);
            return 1;
        }
    }
    return 0;
}
