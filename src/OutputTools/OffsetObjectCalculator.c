#include "builtin.h"

void OffsetCalculatorInit(parser_t *parser)
{
    parser->off = FIRST_ENTRY_POINT + (SectionEntriesCalculator(parser, SECTION_SHSTRTAB_V) * SIZEOF_STRUCT_SECTION_HEADER);
    if (parser->off % 0x10 != 0x0)
        parser->off = (0x10 - (parser->off % 0x10)) + parser->off;
}

/* used to calculate the offset start section in file */
uint32 OffsetCalculator(parser_t *parser, uint size)
{
    if (size == 0x0) {
        if (parser->off % 0x10 != 0x0)
            parser->off = (0x10 - (parser->off % 0x10)) + parser->off;
        return (parser->off);
    }
    if (parser->off % 0x10 != 0x0)
        parser->off = (0x10 - (parser->off % 0x10)) + parser->off;
    parser->off += size;
    return (parser->off - size);
}
