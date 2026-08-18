/*
** cat.c
**
** A custom cut by x4x
** 20250318 x4x
*/

#include <stdio.h>
#include <string.h>

#include "applets.h"

// function to print a file contents
void print_file(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
       fprintf(stderr, "Unable to open file %s\n", filename);
       return;
    }

    //read and print the file
    char ch;
    while ((ch = fgetc(file)) != EOF) {
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

int cat_main(int argc, char* argv[])
{
    //FILE* file;

    //check if filename is given
    if (argc < 2) {
        printf("Usage: %s filename1 [filename2 ...]\n",
               argv[0]);
        printf("       %s - filename  # write to file\n",
               argv[0]);
        printf("              # exit CTL+D\n");
        printf("       %s dest_file - source_fiel  # append to second file\n",
               argv[0]);
        return 1;
    }

    // call read file
    for (int i = 1; i < argc; i++) {
        // '-' is the write file operator
        if(strcmp(argv[i], "-") == 0) {
            write_to_file(argv[++i]);
        }
        else if(i+1 < argc  &&
               strcmp(argv[i+1], "-") == 0 ) {
                concatenate_files(argv[i], argv[i+2]);
                i+=2;
            } else {
            if(argc > 2) {
                printf("%s :\n", argv[i]);
            }
            print_file(argv[i]);
            printf("\n");
        }
        
    }
    return 0;
}
