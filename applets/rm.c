/*
** rm.c
**
** A custom cut by x4x
** 20250320 x4x
*/

#include <stdio.h>  //remove()
//#include <unistd.h>  //unlink()

#include "applets.h"

int rm_main(int argc, char **argv)
{
    int returnerror = 0;

    if(argc < 2) {
        printf("Usage: rm <files>\n");
        return 1;
    }
    for(int i = 1; i < argc; i++) {
        if(remove(argv[i]) != 0) {
            fprintf(stderr, "Failed to remove file: %s\n", argv[i]);
            returnerror = 1;
        }
    }
    return returnerror;
}
