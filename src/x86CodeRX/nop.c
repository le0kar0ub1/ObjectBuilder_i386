#include "builtin.h"

#define OPCODE_NOP     0x90

void EXEC_NOP(parser_t *parser, uint idx)
{
    idx += 0x0;
    uint8 data[1] = {0x90};
    WriteOpcode(parser, SECTION_TEXT_V, data, 1);
}
