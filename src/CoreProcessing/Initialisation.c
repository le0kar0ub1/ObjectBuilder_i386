#include "builtin.h"
#include "ElfHelper.h"
#include "parser.h"
#include "SymbolDescriptor.h"
#include "string.h"

void init_block(parser_t *parser, uint section)
{
    struct code *ptr = parser->out[section];

    for (; ptr && ptr->next != NULL; ptr = (struct code *)ptr->next);
    ptr->next = malloc(SIZEOF_CODE);
    ptr = (struct code *)ptr->next;
    ptr->out = malloc(0x78);
    ptr->i = 0x0;
    ptr->next = NULL;
}

void Init_CodeSection(parser_t *parser, uint8 sect)
{
    parser->out[sect] = (struct code *)malloc(SIZEOF_CODE);
    parser->out[sect]->out = malloc(SIZEOF_CODE_BLOCK_SIZE);
    parser->out[sect]->next = NULL;
    parser->out[sect]->var = NULL;
    parser->out[sect]->i = 0x0;
}

void SectionStorage_init(parser_t *parser, char *rd)
{
    parser->out = malloc(sizeof(struct code) * MAX_SECTION_NUMBER); /* btw 16 sections max is probably sufficient for a prototype */
    for (uint8 i = 0x0; i < MAX_SECTION_NUMBER; i++)
        parser->out[i] = NULL;
    Init_CodeSection(parser, SECTION_TEXT_V);

    InitSymtabStorer(parser, strdup("\0"), STT_NOTYPE, SYMBOL_SECTION_NO_CARE, 0x0);
    SymtabStorerStore(parser, rd, STT_FILE, SYMBOL_SECTION_NO_CARE, 0x0); /* only init these needed sections now to store symbols & sections */
    parser->var = NULL;

    parser->reloc = NULL;
}

void Parser_Initialisation(parser_t *parser, int fdrd, char *rd)
{
    SectionStorage_init(parser, cut_total_path(rd));
    parser->inp = get_file(fdrd, rd);

    parser->control = malloc(sizeof(struct control));
    parser->control->offreg = malloc(sizeof(bool) * 0x5);

    parser->section = UNDEFINED_SECTION; /* no section declared */
}
