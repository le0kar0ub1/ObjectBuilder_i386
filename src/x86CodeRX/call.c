#include "builtin.h"
#include "opcode.h"
#include "string.h"

extern struct REGISTER_t REGISTER;

void EXEC_CALL_FROM_SYM(parser_t *parser)
{
    uint8 *op = malloc(0x2);

    op[0x0] = 0xE8;
    memset(op + 1, 0xff, 0x4);
    WriteOpcode(parser, SECTION_TEXT_V, op, 0x5);
}

void EXEC_CALL(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) > 0x2)
        pexit("Invalid argument number for Call, need 1 operand");
    uint8 type = Instruction1OperandType(parser, idx);

    if (type == TYPE_OP_IS_OFF || type == TYPE_OP_IS_OFF_FROM_SYM)
        pexit("Sorry, no far Jmp Allowed\n");
    else if (type == TYPE_OP_IS_IMM_FROM_SYM)
        EXEC_CALL_FROM_SYM(parser);
    else
        pexit("Invalid call operand\n");
}
