/*
** A custom sync by x4x
** 20260818 x4x
*/
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