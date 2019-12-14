#include "builtin.h"

uint SectionStrTabEntries(SectionStrtab_t *ptr)
{
    uint i = 0x0;
    SectionStrtab_t *data = ptr;

    for (; data; data = data->next, i++);
    return (i);
}

uint SectionShdrEntries(parser_t *parser)
{
    uint i = 0x0;

    i = 0x5; // base needed
    if (parser->out[SECTION_DATA_V])
        i += 0x1;
    if (parser->reloc)
        i += 0x1;
    return (i);
}

uint SectionSymTabEntries(SectionSymtab_t *ptr)
{
    uint i = 0x0;
    SectionSymtab_t *data = ptr;

    for (; data; data = data->next, i++);
    return (i);
}

uint SectionRelocEntries(SectionRelocation_t *rel)
{
    uint i = 0x0;
    SectionRelocation_t *r = rel;

    for (; r; r = r->next, i++);
    return (i);
}

uint SectionEntriesCalculator(parser_t *parser, uint8 section)
{
    switch (section) {
        case SECTION_SHDR_V:
            return (SectionShdrEntries(parser));
            break;
        case SECTION_STRTAB_V:
            return (SectionStrTabEntries((SectionStrtab_t *)parser->str));
            break;
        case SECTION_SYMTAB_V:
            return (SectionSymTabEntries(parser->sym));
            break;
        case SECTION_SHSTRTAB_V:
            return (SectionStrTabEntries(parser->sec));
            break;
        case SECTION_RELOC_V:
            return (SectionRelocEntries(parser->reloc));
        default:
            pexit("Invalid section when try to count entries\n");
    }
    return (0x0);
}
