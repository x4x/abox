/*
** A custom sync by x4x
** 20260818 x4x
*/
#define _DEFAULT_SOURCE  // sync is not in the _POSIX_C_SOURCE
//#include <stdio.h>
#include <unistd.h>

#include "applets.h"

int sync_main(int argc, char **argv)
{
    (void)argc; // unused
    (void)argv; // unused
    sync();
    return 0;
}