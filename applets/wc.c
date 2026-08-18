/*
** wc.c
** a minimal POSIX wc comand.
**
** usage: wc [-c|-m][-lw] [file...]
**
** Required behavior:
** - Read one or more input files (or stdin if no file is given)
** - By default, write the number of newlines, words, and bytes for each file
** - If more than one file is given, also write a total count
** - A "word" is a non-zero-length string delimited by whitespace
**
** Options:
** -c: Count the number of bytes in each file
** -m: Count the number of characters in each file
** -l: Count the number of newlines in each file
** -w: Count the number of words
**
** A custom cut by x4x
** 20260810 x4x
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define BUFERSIZE 4096

typedef struct
{
    unsigned long long lines;
    unsigned long long words;
    unsigned long long characters;
    unsigned long long bytes;
} counts_t;

static int count_stream( FILE *fp, counts_t *c) {
    static unsigned char buffer[BUFERSIZE];
    bool inword = false;

    memset(c, 0, sizeof(*c));



    return 0;
}

int wc_main(int argc, char* argv[]) {
    bool flag_lines = false,
         flage_words = false,
         flage_characters = false,
         flage_byte = false;
    int active_arg = 1;
    
    // get leading arguments
    while(  argc > active_arg &&
            argv[active_arg][0] == '-' &&  // is param
            argv[active_arg][1] != '\0'  // is not stdin mode
            ) {
        //printf("working %i/%i, %s \n", active_arg, argc, argv[active_arg] +1 );
        for(const char *p = argv[active_arg] + 1; *p; p++) {
            switch (*p)
            {
            case 'l':
                flag_lines = true; break;
            case 'w':
                flage_words = true; break;
            case 'm':
                flage_characters = true; break;
            case 'c':
                flage_byte = true; break;
            default:
                fprintf(stderr, "invalid option: %c\n", *p, strerror(errno));
                return(1);
            }
            //printf("point: %c\n", *p);
        }
        active_arg ++;
    }
    // default if no paramter selected
    if(!flag_lines && !flage_byte && !flage_characters && !flage_byte) {
        flag_lines = flage_words = flage_byte = true;
    }
    

    // stdin mode
    if( argc - active_arg == 0 ||
        strcmp(argv[active_arg],"-")) {

    }

    // file mode
    while(argc > active_arg )
    {
        printf("file: %s\n", argv[active_arg]);
        active_arg ++;
    }

    fprintf(stderr, "wc: not implemented\n", strerror(errno));
    return 0;
}