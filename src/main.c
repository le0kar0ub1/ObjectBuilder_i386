#include "builtin.h"
#include "ElfHelper.h"
#include "stdio.h"

char *cut_total_path(char *s)
{
    uint i = len(s) - 0x1, k = 0x0;

    if (my_strstr(s, "/") == 0x0)
        return (s);
    char *new = malloc(i + 0x1);
    for (; i > 0x0 && s[i] != '/'; i--);
    for (i++; s[i]; i++, k++)
        new[k] = s[i];
    new[k] = 0x0;
    return (new);
}

void print_help(void)
{
    sput("./ObjectBuilder $SOURCEFILE.\n");
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 0x2)
        pexit("Bad input\n./Objectbuilder --help\n");
    if (argc == 0x2 && my_strcmp(argv[0x1], "--help"))
        print_help();
    for (uint i = 0x1; argv[i]; i++) {
        if (argv[i][0] == '-')
            pexit("Bad argument, only files are allowed\n");
        start_parser(argv[i]);
    }
    return (0);
}
