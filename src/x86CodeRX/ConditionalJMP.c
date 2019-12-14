#include "builtin.h"
#include "opcode.h"
#include "string.h"
#include "operations_mixing.h"
#include "jmp.h"

extern struct REGISTER_t REGISTER;

uint8 CheckConditionalJMP(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) != 0x2)
        pexit("Invalid argument number for Jmp, need 1 operand\n");
    uint8 type = Instruction1OperandType(parser, idx);
    if (type != TYPE_OP_IS_IMM && type != TYPE_OP_IS_IMM_FROM_SYM)
        pexit("Conditional jmp permit only relative jmp");
    return (type);
}

void WrConditionalJMP(parser_t *parser, uint idx, uint8 type, uint8 *op)
{
    if (type == TYPE_OP_IS_IMM) {
        if (!VGetNbr8(parser->inp[idx][0x1]))
            pexit("Relative jmp allowed a maximum of 128B in memory...\n");
        op[0] = GetNbr8(parser->inp[idx][0x1]);
    } else
        memset(op + 0x1, 0xff, 0x1);
    WriteOpcode(parser, SECTION_TEXT_V, op, 0x2);
}

void EXEC_JE(parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x2);
    uint8 type = CheckConditionalJMP(parser, idx);

    op[0] = JE8;
    // op[1] = JEH2;
    WrConditionalJMP(parser, idx, type, op);
}

void EXEC_JNE(parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x2);
    uint8 type = CheckConditionalJMP(parser, idx);

    op[0] = JNE8;
    // op[1] = JNEH2;
    WrConditionalJMP(parser, idx, type, op);
}

void EXEC_JB(parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x2);
    uint8 type = CheckConditionalJMP(parser, idx);

    op[0] = JB8;
    // op[1] = JBH2;
    WrConditionalJMP(parser, idx, type, op);
}

void EXEC_JBE(parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x2);
    uint8 type = CheckConditionalJMP(parser, idx);

    op[0] = JBE8;
    // op[1] = JBEH2;
    WrConditionalJMP(parser, idx, type, op);
}

void EXEC_JL(parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x2);
    uint8 type = CheckConditionalJMP(parser, idx);

    op[0] = JL8;
    // op[1] = JLH2;
    WrConditionalJMP(parser, idx, type, op);
}

void EXEC_JLE(parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x2);
    uint8 type = CheckConditionalJMP(parser, idx);

    op[0] = JLE8;
    // op[1] = JLEH2;
    WrConditionalJMP(parser, idx, type, op);
}
