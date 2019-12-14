#include "builtin.h"

#define OPCODE_SYSCALL 0x0F34
#define OPCODE_INT     0xCD

void EXEC_INT(parser_t *parser, uint idx)
{
    idx += 0x0;
    uint8 *data = malloc(sizeof(uint8) * 0x2);

    if (parser->inp[idx][0x1] == NULL)
        pexit("int Instruction need an number as argument\n");
    data[0] = OPCODE_INT;
    data[1] = GetNbr8(parser->inp[idx][0x1]);
    WriteOpcode(parser, SECTION_TEXT_V, data, 0x2);
}

void EXEC_SYSCALL(parser_t *parser, uint idx)
{
    idx += 0x0;
    uint8 data[2] = {0xF, 0x34};
    WriteOpcode(parser, SECTION_TEXT_V, data, 0x2);
}
