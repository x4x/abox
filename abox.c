/*
** abox.c
**
** A custom cut by x4x
** 20260719 x4x
*/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include "applets.h"

struct applet {
    const char *name;
    int (*mainfunc)(int argc, char **argv);
};

static struct applet applets[] = {
    {"cat", cat_main},
    {"ls", ls_main},
    {"true", true_main},
    {"false", false_main},
    {"rm", rm_main},
    {NULL, NULL}
};

static const struct applet *find_applet(const char *name)
{
    for (const struct applet *a = applets; a->name != NULL; a++) {
        if (strcmp(a->name, name) == 0) {
            return a;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    char *base = basename(argv[0]);
    const struct applet *applettorun;
  
    if(strcmp(base, "abox") == 0 || strcmp(base, "busybox") == 0)  {
        // if program is called with abox or bussy box
        if (argc < 2) {
            // no applet parameter
            printf("usage: %s <applet> [args...]\n", base);
            printf("applets:\n");
            for (const struct applet *a = applets; a->name != NULL; a++) {
                printf("  %s\n", a->name);
            }
            return 127;
        } else {
            // find applet to run
            applettorun = find_applet(argv[1]);
            if (applettorun == NULL) {
                printf("Unknown applet: %s\n", argv[1]);
                return 127;
            } else {
                // run applet
                return applettorun->mainfunc(argc - 1, argv + 1);
            }
        }
    } else {
        // if program is called with applet name
        applettorun = find_applet(base);
        if (applettorun == NULL) {
            printf("Unknown applet: %s\n", base);
            return 127;
        } else {
            // run applet
            return applettorun->mainfunc(argc, argv);
        }
    }
    return 1;
}