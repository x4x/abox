/*
** cat.c
**
** A custom cat by x4x
** 20250318 x4x
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "applets.h"

static bool flag_linenumbers = false;

// function to print a file contents
void print_file(const char* filename)
{
    FILE *file;
    if(strcmp(filename, "-") == 0) {
        file = stdin;
    } else {
        file = fopen(filename, "r");
    }
    if (file == NULL) {
       fprintf(stderr, "Unable to open file %s\n", filename);
       return;
    }

    unsigned int linenumber = 0;
    //read and print the file
    char ch;
    char chold;
    if(flag_linenumbers) {  // first line
        printf("%i ", linenumber++);
    }
    while ((ch = fgetc(file)) != EOF) {
        chold = ch;
        if(flag_linenumbers && chold == '\n') {
            printf("\n%i ", linenumber++);
            continue;
        }
        putchar(ch);
    }

    fclose(file);
}

void write_to_file(const char* filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
       fprintf(stderr, "Unable to open file %s\n", filename);
       return;
    }

    //read and print the file
    char ch;
    while ((ch = getchar()) != EOF) {
        fputc(ch, file);
    }

    fclose(file);
}

// Function to concatenate the contents of two files
void concatenate_files(const char* filename1,
                       const char* filename2)
{
    FILE *file1 = fopen(filename1, "r+");
    if (file1 == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename1);
        return;
    }

    FILE *file2 = fopen(filename2, "r");
    if (file2 == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename1);
        return;
    }

    fseek(file1, 0, SEEK_END);

    char ch;
    while ((ch = fgetc(file2)) != EOF) {
        fputc(ch, file1);
    }
    
    fclose(file1);
    fclose(file2);
}

void cat_print_help(char* app_name) {
    printf("Usage: %s -n <file1>  # show line numbers\n", app_name);
    printf("       %s <file1> [<file2> ...]\n", app_name);
    printf("       %s - <file>  # write to file\n", app_name);
    printf("              # exit CTL+D\n");
    printf("       %s <dest_file> - <source_fiel>  # append to second file\n", app_name);
}

int cat_main(int argc, char* argv[])
{
    // parameters check
    int active_arg = 1;
    if(argc > 1) {
        if(strcmp(argv[active_arg], "-h") == 0) {
            cat_print_help(argv[0]);
            return 1;
        }
        if(strcmp(argv[active_arg], "-n") == 0) {
            active_arg++;
            flag_linenumbers = true;
        }
    }
    //check if filename is given
    if(argc <= active_arg) {
        print_file("-");
        return 0;
    }

    // call read file
    for (; active_arg < argc; active_arg++) {
        // '-' is the write file operator
        if(strcmp(argv[active_arg], "-") == 0) {
            write_to_file(argv[++active_arg]);
        }
        else if(active_arg+1 < argc  &&
               strcmp(argv[active_arg+1], "-") == 0 ) {
                concatenate_files(argv[active_arg], argv[active_arg+2]);
                active_arg+=2;
            } else {
            if(argc > 2) {
                printf("%s :\n", argv[active_arg]);
            }
            print_file(argv[active_arg]);
            printf("\n");
        }
    }

    return 0;
}
