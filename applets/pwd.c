/* 
** pwd.c
**
** A custom pwd by x4x
** 20250321 x4x
*/

#include <stdio.h>
#include <unistd.h>

#include "applets.h"

int pwd_main(int argc, char **argv)
{
    (void)argc; // Unused parameter
    (void)argv; // Unused parameter

    char path[1024];
    if (getcwd(path, sizeof(path)) == NULL) {
        fprintf(stderr ,"Failed to get current directory\n");
        return 1;
    }
    printf("%s\n", path);
    return 0;
}