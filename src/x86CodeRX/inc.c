#include "builtin.h"
#include "opcode.h"
#include "string.h"
#include "operations_mixing.h"
#include "Inc_Dec.h"

extern struct REGISTER_t REGISTER;

void EXEC_INC_OFF(parser_t *parser, uint idx, uint8 type)
{
    uint8 *op = malloc(0xA);
    uint8 k = 0x0;

    if (type != TYPE_OP_IS_OFF_FROM_SYM)
        parser->inp[idx][0x1] = RemoveBrackets(parser->inp[idx][0x1]); // we already know the instr type
    if (parser->control->OffSize[0x0] == BYTE_VALUE) {
        op[k] = INC_OFFSET8;
        op[k + 1] = 0x5;
        k += 2;
        if (type == TYPE_OP_IS_OFF_FROM_SYM)
            memset(op + k, 0xff, 0x4);
        else
            op = NumberDumperS32(parser->inp[idx][0x1], k, op);
        k += 4;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    } else {
        if (parser->control->OffSize[0x0] == WORD_VALUE) {
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        op[k] = INC_OFFSETH;
        op[k + 1] = 0x5;
        k += 0x2;
        if (type == TYPE_OP_IS_OFF_FROM_SYM)
            memset(op + k, 0xff, 0x4);
        else
            op = NumberDumperS32(parser->inp[idx][0x1], k, op);
        k += 4;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_INC_REG(parser_t *parser, uint8 type)
{
    uint8 *op = malloc(0x2);
    uint8 k = 0x0;

    if (type % 10 == REGISTER_8b) {
        op[k] = INC_REGISTER8;
        op[k + 1] = BASE_INC_REGISTER8 + (type / 10);
        WriteOpcode(parser, SECTION_TEXT_V, op, 0x2);
    } else {
        if (type % 10 == REGISTER_16b) {
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        op[k] = INC_REGISTERH + (type / 10);
        k++;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_INC_OFFREG(parser_t *parser, uint idx, uint8 type)
{
    uint8 *op = malloc(0x3);
    uint8 k = 0x0;

    if (type % 10 != REGISTER_32b)
        pexit("Invalid register size on Inc instruction\n");
    parser->inp[idx][0x1] = RemoveBrackets(parser->inp[idx][0x1]); // we already know the instr type
    if (parser->control->OffSize[0x0] == BYTE_VALUE) {
        op[k] = INC_OFFSET8;
        op[k + 1] = type / 10;
        WriteOpcode(parser, SECTION_TEXT_V, op, 0x2);
    } else {
        if (parser->control->OffSize[0x0] == WORD_VALUE) {
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        op[k] = INC_OFFSETH;
        op[k + 1] = type / 10;
        k += 2;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_INC(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) > 0x3)
        pexit("Invalid argument number for Inc, need 1 operand\n");
    uint8 type = Instruction1OperandType(parser, idx);

    if ((type == TYPE_OP_IS_IMM && parser->control->OffSize[0x0] != UNDEFINED_OFF_VAL) ||
(type < TYPE_OP_IS_REG_MAX && !parser->control->offreg[0x0] && parser->control->OffSize[0x0] != UNDEFINED_OFF_VAL))
        pexit("Invalid argument when assign a size in front of an operand wich is not an offset\n");
    if (type == TYPE_OP_IS_IMM || type == TYPE_OP_IS_IMM_FROM_SYM)
        pexit("Can't increment an Immediate value\n");

    if (type == TYPE_OP_IS_OFF || type == TYPE_OP_IS_OFF_FROM_SYM)
        EXEC_INC_OFF(parser, idx, type);
    if (type < TYPE_OP_IS_REG_MAX && parser->control->offreg[0x0])
        EXEC_INC_OFFREG(parser, idx, type);
    if (type < TYPE_OP_IS_REG_MAX && !parser->control->offreg[0x0])
        EXEC_INC_REG(parser, type);
}
