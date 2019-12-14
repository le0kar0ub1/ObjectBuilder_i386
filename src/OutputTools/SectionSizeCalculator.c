#include "builtin.h"

uint SectionStrTabSize(SectionStrtab_t *ptr)
{
    uint i = 0x1;
    SectionStrtab_t *data = ptr;

    for (; data; data = data->next)
        i += len(data->name) + 0x1;
    return (i - 0x1); // last string havn't space
}

uint SectionShdrSize(parser_t *parser)
{
    uint i = 0x0;

    i += 40 * 5; // base needed
    if (parser->out[SECTION_DATA_V])
        i += 40;
    return (i);
}

uint SectionSymTabSize(SectionSymtab_t *ptr)
{
    uint i = 0x0;
    SectionSymtab_t *data = ptr;

    for (; data; data = data->next)
        i += 0x10;
    return (i);
}

uint ElfSpec_SectionSizeCalculator(parser_t *parser, uint8 section)
{
    switch (section) {
        case SECTION_SHSTRTAB_V:
            return (SectionStrTabSize(parser->sec));
            break;
        case SECTION_STRTAB_V:
            return (SectionStrTabSize((SectionStrtab_t *)parser->str));
            break;
        case SECTION_SYMTAB_V:
            return (SectionSymTabSize(parser->sym));
            break;
        case SECTION_SHDR_V:
            return (SectionShdrSize(parser));
            break;
        default:
            pexit("Invalid section when try to count size\n");
    }
    return (0x0);
}

uint CodeSectionSize(struct code *ptr)
{
    struct code *data = ptr;
    uint i = 0x0;

    for (; data; data = data->next)
        i += data->i;
    return (i);
}

uint SectionSizeCalculator(parser_t *parser, uint8 section)
{
    if (section < MAX_SECTION_NUMBER) {
        switch (section) {
            case SECTION_TEXT_V:
                return (CodeSectionSize(parser->out[SECTION_TEXT_V]));
                break;
            case SECTION_DATA_V:
                return (CodeSectionSize(parser->out[SECTION_DATA_V]));
                break;
            default:
                pexit("Unmanaged section runtime\n");
        }
    } else {
        return (ElfSpec_SectionSizeCalculator(parser, section));
    }
    return (0x0);
}
