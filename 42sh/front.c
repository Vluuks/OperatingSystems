#include "parser.h"
#include "lexer.h"
#include "lex.yy.h"
#include "xalloc.h"
#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "front.h"

char *prompt = 0;
extern int echo, parse_error; // from the parser
mc *all_mem;


void delete_all_mem(void) {
    free_all_mem(all_mem);
}
extern inline void register_global_mem(void *pt, const free_fun fun)
{
    register_mem(all_mem, pt, fun);
}
extern inline void free_from_global_mem(void *pt)
{
    free_mem(all_mem, pt);
}

void free_the_parser(void *parser)
{
    ParseFree(parser, free);
}
void my_yy_delete_buffer(void *st)
{
    yy_delete_buffer((YY_BUFFER_STATE)st);
}



static void handle_command(char *cmd)
{
    void *parser;
    int yv;
    struct lex_token tok;
    YY_BUFFER_STATE st;

    // prepare a parser context.
    parser = ParseAlloc(malloc);
    register_global_mem(parser, &free_the_parser);
    parse_error = 0;

    // prepare a lexer context.
    st = yy_scan_string(cmd);
    register_global_mem(st, &my_yy_delete_buffer);
    
    // while there are some lexing tokens...
    while ( (yv = yylex()) != 0 )
    {
        tok.text = 0;
        tok.number = -1;

        // NUMBER and WORD are the only 2 token types
        // with a carried value.
        if (yv == NUMBER || yv == WORD)
        {
            tok.text = xstrdup(token_text);
            if (yv == NUMBER)
                tok.number = atoi(tok.text);
        }

        // process the token in the parser.
        Parse(parser, yv, tok);

        // if at end, finish the parsing.
        if (yv == END)
            break;
    }

    // complete parse.
    Parse(parser, 0, tok);

    // release resources.
    free_from_global_mem(parser);
    free_from_global_mem(st);
}

void my_yylex_destroy(void) {
    yylex_destroy();
}

int main(int argc, char *argv[])
{
    int opt;
    int save_history = 0;

    all_mem = init_mc();
    atexit(&delete_all_mem);
    
    // command-line argument parsing.

    while ((opt = getopt(argc, argv, "hec:")) != -1)
    {
        switch(opt) {
        case 'h':
            printf( "usage: %s [OPTS] [FILE]\n"
                    "options:\n"
                    " -h      print this help.\n"
                    " -e      echo commands before running them.\n"
                    " -c CMD  run this command then exit.\n"
                    " FILE    read commands from FILE.\n",
                    argv[0]);
            return EXIT_SUCCESS;

        case 'e':
            echo = 1;
            break;

        case 'c':
            initialize();
            handle_command(optarg);
            return 0;

        }
    }

    // reading commands from a script or stdin.

    if (optind >= argc)
    {
        // reading from stdin; handle history if terminal.

        if (isatty(0))
        {
            using_history();
            read_history(0);
            prompt = "42sh$ ";
            save_history = 1;
        }
    }
    else
    {
        // reading from file.

        FILE *f = fopen(argv[optind], "r");
        if (f == 0)
        { perror(argv[optind]); exit(1); }
        rl_instream = f;
        prompt = 0;
    }

    // the main loop.

    initialize();
    char *line;
    while ((line = readline(prompt)) != 0)
    {
        register_global_mem(line, &free);
        if (save_history && line[0] != '\0')
        {
            add_history(line);
            write_history(0);
        }
        handle_command(line);
        free_from_global_mem(line);
    }

    return 0;
}
