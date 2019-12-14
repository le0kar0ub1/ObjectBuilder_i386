#include "builtin.h"
#include "opcode.h"
#include "string.h"
#include "operations_mixing.h"
#include "jmp.h"

extern struct REGISTER_t REGISTER;

void EXEC_JMP_FROM_IMM(parser_t *parser, uint idx, uint8 type)
{
    uint8 *op = malloc(0x2);

    op[0x0] = OPCODE_JMP_REL8;
    if (type == TYPE_OP_IS_IMM) {
        if (!VGetNbr8(parser->inp[idx][0x1]))
            pexit("Relative jmp allowed a maximum of 128B in memory...\n");
        op[0x1] = GetNbr8(parser->inp[idx][0x1]);
    } else
        op[0x1] = 0xff; //memset(op + 0x1, 0xff, 0x1);
    WriteOpcode(parser, SECTION_TEXT_V, op, 0x2);
}

void EXEC_JMP_FROM_REG(parser_t *parser, uint8 type)
{
    uint8 *op = malloc(0x3);

    if (type % 10 == REGISTER_8b)
        pexit("Can't jump on 8b registers\n");
    if (type % 10 == REGISTER_16b) {
        op[0] = OPERAND_SIZE_PREFIX;
        op[1] = OPCODE_JMP_ABSH;
        op[2] = OPCODE_JMP_REGS_BASE + (type / 0xA);
        WriteOpcode(parser, SECTION_TEXT_V, op, 0x3);
    } else {
        op[0] = OPCODE_JMP_ABSH;
        op[1] = OPCODE_JMP_REGS_BASE + (type / 0xA);
        WriteOpcode(parser, SECTION_TEXT_V, op, 0x2);
    }
}

void EXEC_JMP(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) > 0x2)
        pexit("Invalid argument number for Jmp, need 1 operand");
    uint8 type = Instruction1OperandType(parser, idx);

    if (type == TYPE_OP_IS_OFF || type == TYPE_OP_IS_OFF_FROM_SYM)
        pexit("Sorry, no far Jmp Allowed\n");
    if (type == TYPE_OP_IS_IMM || type == TYPE_OP_IS_IMM_FROM_SYM)
        EXEC_JMP_FROM_IMM(parser, idx, type);
    if (type < TYPE_OP_IS_REG_MAX)
        EXEC_JMP_FROM_REG(parser, type);
}
