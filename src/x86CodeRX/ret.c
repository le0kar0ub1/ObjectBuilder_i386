#include "builtin.h"
#include "opcode.h"

extern struct REGISTER_t REGISTER;

#define OPCODE_RET 0xC3

void EXEC_RET(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) != 0x1)
        pexit("Invalid operand for ret, need 0 argument\n");
    uint8 op = OPCODE_RET;
    WriteOpcode(parser, SECTION_TEXT_V, &op, 0x1);
}
