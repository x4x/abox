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
#include <ctype.h>
//#include <errno.h>

#include "applets.h"

#define BUFERSIZE 4096

typedef struct
{
    unsigned long long lines;
    unsigned long long words;
    unsigned long long characters;
    unsigned long long bytes;
} counts_t;

static bool flag_lines = false,
            flag_words = false,
            flag_characters = false,
            flag_bytes = false;

static int count_stream( FILE *fp, counts_t *c) {
    static unsigned char buffer[BUFERSIZE];
    size_t n;  // byte count
    bool inword = false;

    memset(c, 0, sizeof(*c));  // set counters to 0

    while((n = fread(buffer, 1,  BUFERSIZE, fp)) > 0 )
    {
        c->bytes += n;
        for(size_t i = 0; i < n; i++) {
            if( buffer[i] == '\n' ) {
                c->lines ++;
            }
            if( isspace(buffer[i]) ) {
                inword = false;
            } else if( !inword ) {
                c->words ++;
                inword = true;
            }
        }
    }
    return 0;
}

void print_counts(counts_t c, const char *filename) {
    if (flag_lines)
        printf("%8llu", c.lines);
    if (flag_words)
        printf("%8llu", c.words);
    if (flag_bytes)
        printf("%8llu", c.bytes);
    if (filename != NULL)
        printf(" %s", filename);
    putchar('\n');
}

int wc_main(int argc, char* argv[]) {
    int active_arg = 1;
    counts_t total = {0};
    
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
                flag_words = true; break;
            case 'm':
                flag_characters = true;
                printf("Not implemented\n"); return 1;
                break;
            case 'c':
                flag_bytes = true; break;
            default:
                fprintf(stderr, "invalid option: %c\n", *p);
                return(1);
            }
            //printf("point: %c\n", *p);
        }
        active_arg ++;
    }
    // default if no paramter selected
    if(!flag_lines && !flag_words && !flag_characters && !flag_bytes) {
        flag_lines = flag_words = flag_bytes = true;
    }
    int cnt_options = active_arg;
    
    // iterrate over files
    while(argc > active_arg )
    {
        counts_t c;
        FILE *fp;

        // stdin mode
        if( strcmp(argv[active_arg],"-") == 0 ) {
                fp = stdin;
        } else {
            // file mode
            //printf("file: %s\n", argv[active_arg]);
            fp = fopen(argv[active_arg], "rb");  // binary read
            if (fp == NULL) {
                fprintf(stderr, "Unable to open file %s\n", argv[active_arg]);
                active_arg++;
                continue;  // skip this file
            }

            count_stream(fp, &c);
            print_counts(c, argv[active_arg]);

            if (fp != stdin) {
                fclose(fp);
            }
            total.lines += c.lines;
            total.words += c.words;
            total.bytes += c.bytes;

            active_arg ++;
        }
    }

    if( active_arg - cnt_options > 1 ) {
        print_counts(total, "total");
    }
    
    //fprintf(stderr, "wc: not implemented\n");
    return 0;
}