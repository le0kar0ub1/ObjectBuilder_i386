#include "SymbolDescriptor.h"
#include "builtin.h"

uint modify_running_section(parser_t *parser, uint idx)
{
    if (my_strcmp(parser->inp[idx][1], SECTION_TEXT) == 0x1)
        return (SECTION_TEXT_V);
    if (my_strcmp(parser->inp[idx][1], SECTION_DATA) == 0x1) {
        if (!(parser->out[SECTION_DATA_V]))
            Init_CodeSection(parser, SECTION_DATA_V);
        return (SECTION_DATA_V);
    }
    // if (my_strcmp(parser->inp[idx][1], SECTION_BSS) == 0x1)
    //     return (SECTION_BSS_V);
    // if (my_strcmp(parser->inp[idx][1], SECTION_RODATA) == 0x1)
    //     return (SECTION_RODATA_V);
    pexit("invalid section\n");
    return (0x0);
}

void modify_running_label(parser_t *parser, uint idx)
{
    parser->inp[idx][0][len(parser->inp[idx][0]) - 1] = 0x0;
    if (!sis_alpha(parser->inp[idx][0]) || (parser->inp[idx][1] != NULL && parser->inp[idx + 1]))
        pexit("invalid label\n");
    uint sz = SectionSizeCalculator(parser, parser->section);
    SymtabStorerStore(parser, parser->inp[idx][0], STT_NOTYPE, parser->section, sz);
}
