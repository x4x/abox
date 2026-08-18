/*
** ls.c
**
** A custom cut by x4x
** 20250320 x4x
*/

#include <stdio.h>
#include <dirent.h>

#include "applets.h"


static int list_dir(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Failed to open directory: %s\n", path);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}

int ls_main(int argc, char **argv)
{
    if (argc < 2) {
        return list_dir(".");
    }

    for (int i = 1; i < argc; i++) {
        if (list_dir(argv[i]) != 0) {
            return 1;
        }
    }
    return 0;
}

