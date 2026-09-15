/*
** find.c
**
** A custom find by x4x
** 20260914 x4x
**
** find [PATH] [-name PATTERN] [-type f|d|df] [-maxdepth n]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <fnmatch.h>

#include "applets.h"

struct Flags {
    char* path;
    char* pattern;
    bool search_file;
    bool search_dir;
    int maxdepth;
};

static struct Flags flags= {
    .path        = ".",
    .pattern     = "*",
    .search_file = true,
    .search_dir  = false,
    .maxdepth     = -1,
};

void print_help() {
    printf("Usage: find [PATH] [-name PATTERN] [-type f|d|df] [-maxdepth n]\n");
    printf("ther is currently just a singe argument for each param supported!\n");
}

static int search_dir(const char* path, int current_deth) {
    char child_path[4096];

    if( flags.search_dir &&
        strcmp(flags.pattern, "*") == 0
    ) {
        printf("%s\n", path); // print curent path eg '.'
    }
    DIR *dir = opendir(path);

    if (dir == NULL) {
        fprintf(stderr, "Failed to open directory: %s\n", path);
        return 1;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        // avoid recusion in to self or parrent.
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // get full child info
        int r = snprintf(child_path, sizeof(child_path),
                         "%s/%s", path, entry->d_name);

        if (r < 0 || r >= (int)sizeof(child_path)){
            fprintf(stderr, "Path to long: %s/%s", path, entry->d_name);
            continue;
        }
        
        if(lstat(child_path, &st) == -1) {
            fprintf(stderr, "Failed to get info for: %s in %s\n", entry->d_name, path);
            continue; 
        }
        
        //printf("%s\n", child_path);

        // filtering
        bool is_match = false;
        if(flags.search_file && S_ISREG(st.st_mode)) {
            is_match = true;
        }
        if(flags.search_dir && S_ISDIR(st.st_mode)) {
            is_match = true;
        }

        if(is_match &&
           fnmatch(flags.pattern, entry->d_name, 0) == 0  // patern match
        ) {
            printf("%s\n", child_path);
        }

        if (S_ISDIR(st.st_mode) &&
            ( flags.maxdepth < 0 || flags.maxdepth >= current_deth )
        ) {
            search_dir(child_path, current_deth +1);
        }
    }

    closedir(dir);
    return 0;
}

int find_main(int argc, char** argv)
{
    int active_arg = 1;

    // check for args 
    if(argc > 1) {
        // get path from params
        if(argv[active_arg][0] != '-') {
            flags.path = argv[active_arg];
            active_arg++;
        }

        // get search parameters
        while(argc > active_arg &&
              argv[active_arg][0] == '-' // is param
        ) {
            if(strcmp(argv[active_arg], "-help") == 0) {
                print_help();
            }else if(strcmp(argv[active_arg], "-name") == 0) {
                active_arg++;
                if(argc <= active_arg ) {
                    printf("name requers a argument\n");
                    return 1;
                }
                flags.pattern = argv[active_arg];
            }else if (strcmp(argv[active_arg], "-type") == 0) {
                active_arg++;
                if(argc <= active_arg) {
                    printf("type requers a argument\n");
                    return 1;
                }
                bool has_f = false;
                for(int i=0; argv[active_arg][i] != '\0'; i++) {
                    if(argv[active_arg][i] == 'f') {
                        flags.search_file = true;
                        has_f = true;
                    }else if(argv[active_arg][i] == 'd') {
                        flags.search_dir = true;
                        if(!has_f) {
                            flags.search_file = false;
                        }
                    }else {
                        printf("Unsuported param\n");
                        print_help();
                        return 1;
                    }
                }
            }else if(strcmp(argv[active_arg], "-maxdepth") == 0) {
                active_arg++;
                if(argc <= active_arg) {
                    printf("maxdepth requers a argument\n");
                    return 1;
                }
                flags.maxdepth = strtol(argv[active_arg], NULL, 10);
            } else {
                printf("Unknown param\n");
                print_help();
                return 1;
            }
            active_arg++;
        }
    }

    // now the search
    search_dir(flags.path, 0);
    
    return 0;
}
