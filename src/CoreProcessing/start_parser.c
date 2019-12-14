#include "builtin.h"
#include "ElfHelper.h"
#include "parser.h"

char *output_file(char *arg)
{
    char *s = malloc(len(arg) + 0x4);
    uint i = 0x0;

    my_strcpy(s, arg);
    for (; arg[i] && arg[i] != 0x2E; i++)
        s[i] = arg[i];
    s[i] = '.';
    s[i + 0x1] = 'o';
    s[i + 0x2] = 0x0;
    return (s);
}

void free_data(parser_t *parser)
{
    for (uint i = 0x0; parser->inp[i]; i++)
        for (uint j = 0x0; parser->inp[i][j]; j++)
            free(parser->inp[i][j]);
    uint idx = 0x0;
    for (struct code **fr = parser->out; fr[idx]; idx++) {
        for (struct code *f = fr[idx]; f && f->next != NULL; f = (struct code *)f->next) {
            free(f->out);
        }
    }
    free(parser);
}

/* main loop parsing the code */
void start_parser(char *rd)
{
    int fdrd, fdwr;
    parser_t *parser = malloc(sizeof(parser_t));

    if ((fdrd = open(rd, O_RDONLY)) == -0x1)
        pexit("Can't open input file\n");

    Parser_Initialisation(parser, fdrd, rd);
    /* total file parsing loop */
    for (uint idx = 0x0; parser->inp[idx]; idx++) {
        if (my_strcmp(parser->inp[idx][0], "section") == 1)
            parser->section = modify_running_section(parser, idx);
        else if (parser->inp[idx][0][len(parser->inp[idx][0]) - 1] == ':')
            modify_running_label(parser, idx);
        else if (symbol_ParamDef(parser, idx))
            true;
        else
            running_code(parser, idx);
        /* exit in running_code if parser error occured */
    }
    /* output file */
    if ((fdwr = open(output_file(cut_total_path(rd)), O_CREAT  |
O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) == -0x1)
        pexit("Can't creat output file\n");
    UpdateAndInit(parser);
    dump_data(parser, fdwr);
    // free_data(parser);
}
