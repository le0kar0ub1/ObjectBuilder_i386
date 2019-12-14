#include "builtin.h"
#include "SymbolDescriptor.h"

void InitSymtabStorer(parser_t *parser, char *name, uint8 flag, uint8 section, uint32 memory)
{
    parser->str = malloc(sizeof(SymtabStorer_t));
    parser->str->name = name;
    parser->str->flags = flag;
    parser->str->section = section;
    parser->str->memory = memory;
    parser->str->next = NULL;
}

void SymtabStorerStore(parser_t *parser, char *name, uint8 flag, uint8 section, uint32 memory)
{
    SymtabStorer_t *data = parser->str;

    for (; data->next; data = data->next);
    data->next = malloc(sizeof(SymtabStorer_t));
    data = data->next;
    data->name = name;
    data->flags = flag;
    data->section = section;
    data->memory = memory;
    data->next = NULL;
}

uint SymtabStorerEntriesCalculator(parser_t *parser)
{
    SymtabStorer_t *data = parser->str;
    uint i = 0x0;

    for (; data; data = data->next, i++);
    return (i);
}

uint SymtabStorer_StrIndexFinder(parser_t *parser, char *name)
{
    SymtabStorer_t *data = parser->str;
    uint i = 0x1;

    for (; data; data = data->next) {
        if (my_strcmp(data->name, name))
            return (i - 0x1);
        i += len(data->name) + 0x1;
    }
    pexit("Error when try to find symbol in storer\n");
    return (i);
}

uint SymtabStorerIndexFinder(parser_t *parser, char *cmp)
{
    SymtabStorer_t *data = parser->str;

    for (uint i = 0x0; data; data = data->next, i++)
        if (my_strcmp(data->name, cmp))
            return (i);
    pexit("Error when try to find symbol in the storer\n");
    return (0x0);
}
