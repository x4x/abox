/*
** A custom sync by x4x
** 20260818 x4x
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "applets.h"

int sleep_main(int argc, char **argv)
{
    (void)argc; // unused
    (void)argv; // unused
    if(argc < 2) {
        printf("Usage: sleep <seconds>\n");
        return 1;
    }
    sleep(atoi(argv[1]));
    return 0;
}