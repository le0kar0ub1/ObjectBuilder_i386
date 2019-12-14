#include "builtin.h"
#include "x86_opcode/opcode.h"
#include "x86CodeRX/mov.h"
#include "x86_opcode/operations_mixing.h"
#include "math.h"

extern struct REGISTER_t REGISTER;

void MOV_SRC_REG_DEST_REG(uint8 *type, parser_t *parser)
{
    uint8 *op = malloc(0xA);
    uint8 k = 0x0;

    if (type[TYPE_SRC] % 10 == REGISTER_8b) {
        op[k] = R8_R8;
        op[k + 1] = RegsOpcodeCalculator(type, RegsToRegsBaseValue);
        WriteOpcode(parser, SECTION_TEXT_V, op, 2);
    } else {
        if (type[TYPE_SRC] % 10 == REGISTER_16b) {
            op[k] = OPERAND_SIZE_PREFIX;
            op[k + 1] = RH_RH;
            op[k + 2] = RegsOpcodeCalculator(type, RegsToRegsBaseValue);
            WriteOpcode(parser, SECTION_TEXT_V, op, 3);
        } else {
            op[k] = RH_RH;
            op[k + 1] = RegsOpcodeCalculator(type, RegsToRegsBaseValue);
            WriteOpcode(parser, SECTION_TEXT_V, op, 2);
        }
    }
    free(op);
}

void MOV_SRC_REG_DEST_OFF(uint8 *type, parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x10);
    uint8 k = 0x0;

    if (parser->control->OffSize[TYPE_DEST] == UNDEFINED_OFF_VAL)
        pexit("Sizeof of mov from/to offset unspecified\n");
    if (type[TYPE_SRC] % 10 != parser->control->OffSize[TYPE_DEST] - 1)
        pexit("Invalid Offset size specification\n");

    parser->inp[idx][DEST] = RemoveBrackets(parser->inp[idx][DEST]); // we already know the instr type
    if (type[TYPE_SRC] % 10 == REGISTER_8b) {
        /* Register 8 bits */
        if (type[TYPE_SRC] / 10 == 0x0) {
            op[k] = OFF8_AL;
            k += 1;
        } else {
            op[k] = OFF8_R8;
            op[k + 1] = 0xD + ((type[TYPE_SRC] / 10) * 8) - 0x8;
            k += 2;
        }
        op = NumberDumperS32(parser->inp[idx][DEST], k, op);
        k += 4;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    } else {
        /* Register 16-32 bits */
        if (type[TYPE_SRC] % 10 == REGISTER_16b) {
            /* Register 16 bits prefix and overflow check */
            op[k] = OPERAND_SIZE_PREFIX;
            if (type[TYPE_SRC] / 10 == 0x0) {
                op[k + 1] = OFFH_AX;
                k += 2;
            } else {
                op[k + 1] = OFFH_RH;
                op[k + 2] = 0xD + ((type[TYPE_SRC] / 10) * 8) - 0x8;
                k += 3;
            }
            op = NumberDumperS32(parser->inp[idx][DEST], k, op);
            k += 4;
            WriteOpcode(parser, SECTION_TEXT_V, op, k);
        } else {
            if (type[TYPE_SRC] / 10 == 0x0) {
                op[k] = OFFH_EAX;
                k += 1;
            } else {
                op[k] = OFFH_RH;
                op[k + 1] = 0xD + ((type[TYPE_SRC] / 10) * 8) - 0x8;
                k += 2;
            }
            op = NumberDumperS32(parser->inp[idx][DEST], k, op);
            k += 4;
            WriteOpcode(parser, SECTION_TEXT_V, op, k);
        }
    }
    free(op);
}

void MOV_SRC_IMM_DEST_REG(uint8 *type, parser_t *parser, uint idx)
{
    uint8 *op = malloc(0xA);
    uint8 k = 0x0;

    if (type[TYPE_DEST] % 10 == REGISTER_8b) {
        /* Register 8 bits */
        if (!CheckSize8(parser->inp[idx][SRC]))
            pexit("Overflow, operand max size 8\n");
        op[k] = R8_IMM8 + REGISTER.val[type[TYPE_DEST] % 10];
        op[k + 0x1] = GetNbr8(parser->inp[idx][SRC]);
        WriteOpcode(parser, SECTION_TEXT_V, op, 2);
    } else {
        /* Register 16-32 bits */
        if (type[TYPE_DEST] % 10 == REGISTER_16b) {
            /* Register 16 bits prefix and overflow check */
            if (!CheckSize16(parser->inp[idx][SRC]))
                pexit("Overflow, operand max size 16\n");
            op[k] = OPERAND_SIZE_PREFIX;
            op[k + 1] = RH_IMMH + REGISTER.val[type[TYPE_DEST] / 10];
            k += 2;
            op = NumberDumperS16(parser->inp[idx][SRC], k, op);
            WriteOpcode(parser, SECTION_TEXT_V, op, 6);
        } else {
            op[k] = RH_IMMH + REGISTER.val[type[TYPE_DEST] / 10];
            k += 1;
            op = NumberDumperS32(parser->inp[idx][SRC], k, op);
            WriteOpcode(parser, SECTION_TEXT_V, op, 5);
        }
    }
    free(op);
}

void MOV_SRC_IMM_DEST_OFF(parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x10);
    uint8 k = 0x0;

    if (parser->control->OffSize[TYPE_DEST] == UNDEFINED_OFF_VAL)
        pexit("Sizeof of mov from/to offset unspecified\n");
    if (parser->control->OffSize[TYPE_DEST] == BYTE_VALUE && !VGetNbr8(parser->inp[idx][SRC]))
        print("Bad operand on Mov\n");
    if (parser->control->OffSize[TYPE_DEST] == WORD_VALUE && !VGetNbr16(parser->inp[idx][SRC]))
        print("Bad operand on Mov\n");
    parser->inp[idx][DEST] = RemoveBrackets(parser->inp[idx][DEST]); // we already know the instr type

    if (parser->control->OffSize[TYPE_DEST] == BYTE_VALUE) {
        op[k] = OFF8_IMM8;
        op[k + 1] = 0x5;
        k += 0x2;
        op = NumberDumperS32(parser->inp[idx][DEST], k, op);
        k += 0x4;
        op[k] = GetNbr8(parser->inp[idx][SRC]);
        WriteOpcode(parser, SECTION_TEXT_V, op, 7);
    } else {
        if (parser->control->OffSize[TYPE_DEST] == WORD_VALUE) {
            op[k] = OPERAND_SIZE_PREFIX;
            op[k + 1] = OFFH_IMMH;
            op[k + 2] = 0x5;
            k += 0x3;
            op = NumberDumperS32(parser->inp[idx][DEST], k, op);
            k += 0x4;
            op[k] = GetNbr16(parser->inp[idx][SRC]);
            WriteOpcode(parser, SECTION_TEXT_V, op, 9);
        } else {
            op[k] = OFFH_IMMH;
            op[k + 1] = 0x5;
            k += 0x2;
            op = NumberDumperS32(parser->inp[idx][DEST], k, op);
            k += 0x4;
            op[k] = GetNbr32(parser->inp[idx][SRC]);
            WriteOpcode(parser, SECTION_TEXT_V, op, 10);
        }
    }
    free(op);
}

void MOV_SRC_OFF_DEST_REG(uint8 *type, parser_t *parser, uint idx)
{
    uint8 *op = malloc(0x10);
    uint8 k = 0x0;

    if (parser->control->OffSize[TYPE_SRC] == UNDEFINED_OFF_VAL)
        pexit("Sizeof of mov from/to offset unspecified\n");
    if (type[TYPE_DEST] % 10 != parser->control->OffSize[TYPE_SRC] - 1)
        pexit("Invalid Offset size specification\n");
    parser->inp[idx][SRC] = RemoveBrackets(parser->inp[idx][SRC]); // we already know the instr type

    if (type[TYPE_DEST] % 10 == REGISTER_8b) {
        /* Register 8 bits */
        if (type[TYPE_DEST] / 10 == 0x0) {
            op[k] = AL_OFF8;
            k += 1;
        } else {
            op[k] = R8_OFF8;
            op[k + 1] = 0xD + ((type[TYPE_DEST] / 10) * 8) - 0x8;
            k += 2;
        }
        op = NumberDumperS32(parser->inp[idx][SRC], k, op);
        k += 4;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    } else {
        /* Register 16-32 bits */
        if (type[TYPE_DEST] % 10 == REGISTER_16b) {
            /* Register 16 bits prefix and overflow check */
            op[k] = OPERAND_SIZE_PREFIX;
            if (type[TYPE_DEST] / 10 == 0x0) {
                op[k + 1] = AX_OFFH;
                k += 2;
            } else {
                op[k + 1] = RH_OFFH;
                op[k + 2] = 0xD + ((type[TYPE_DEST] / 10) * 8) - 0x8;
                k += 3;
            }
            op = NumberDumperS32(parser->inp[idx][SRC], k, op);
            k += 4;
            WriteOpcode(parser, SECTION_TEXT_V, op, k);
        } else {
            if (type[TYPE_DEST] / 10 == 0x0) {
                op[k] = EAX_OFFH;
                k += 1;
            } else {
                op[k] = RH_OFFH;
                op[k + 1] = 0xD + ((type[TYPE_DEST] / 10) * 8) - 0x8;
                k += 2;
            }
            op = NumberDumperS32(parser->inp[idx][SRC], k, op);
            k += 4;
            WriteOpcode(parser, SECTION_TEXT_V, op, k);
        }
    }
    free(op);
}

void MOV_SRC_OFFREG_DEST_REG(uint8 *type, parser_t *parser)
{
    uint8 *op = malloc(0x2);

    if (parser->control->OffSize[TYPE_SRC] == UNDEFINED_OFF_VAL)
        pexit("Sizeof of mov from/to offset unspecified\n");
    if (type[TYPE_DEST] % 10 != REGISTER_32b)
        pexit("Invalid Offset size specification\n");
    op[0] = RH_OFFH;
    uint8 tmp = type[0];
    type[0] = type[1];
    type[1] = tmp;
    op [1] = (REGISTER.val[type[TYPE_SRC] / 10] * 8) + (REGISTER.val[type[TYPE_DEST] / 10]);
    WriteOpcode(parser, SECTION_TEXT_V, op, 2);
    free(op);
}

void MOV_SRC_REG_DEST_OFFREG(uint8 *type, parser_t *parser)
{
    uint8 *op = malloc(0x2);

    if (parser->control->OffSize[TYPE_DEST] == UNDEFINED_OFF_VAL)
        pexit("Sizeof of mov from/to offset unspecified\n");
    if (type[TYPE_SRC] % 10 != REGISTER_32b)
        pexit("Invalid Offset size specification\n");
    op[0] = OFFH_RH;
    op[1] = (REGISTER.val[type[TYPE_SRC] / 10] * 8) + (REGISTER.val[type[TYPE_DEST] / 10]);
    WriteOpcode(parser, SECTION_TEXT_V, op, 2);
    free(op);
}

void EXEC_MOV(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) < 0x3 || len_tab(parser->inp[idx]) > 0x4)
        pexit("Invalid argument number for Mov, need 2 operands\n");
    /* check offset/size and get operands types */
    uint8 *Type = Instruction2OperandType(parser, idx);
    parser->control->type = Type;

    if ((Type[TYPE_SRC] == TYPE_OP_IS_IMM && parser->control->OffSize[TYPE_SRC] != UNDEFINED_OFF_VAL) ||
(Type[TYPE_DEST] == TYPE_OP_IS_IMM && parser->control->OffSize[TYPE_DEST] != UNDEFINED_OFF_VAL) ||
(Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && !parser->control->offreg[TYPE_SRC] && parser->control->OffSize[TYPE_SRC] != UNDEFINED_OFF_VAL) ||
(Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX && !parser->control->offreg[TYPE_DEST] && parser->control->OffSize[TYPE_DEST] != UNDEFINED_OFF_VAL))
        pexit("Invalid argument when assign a size in front of an operand wich is not an offset\n");
    /* Check operand validity */
    if (Type[TYPE_SRC] == TYPE_OP_IS_IMM && Type[TYPE_DEST] == TYPE_OP_IS_IMM)
        pexit("Two operands are Immediate values\n");
    if (Type[TYPE_SRC] == TYPE_OP_IS_OFF && Type[TYPE_DEST] == TYPE_OP_IS_OFF)
        pexit("Two operands are Offset values\n");
    if (Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX &&
Type[TYPE_SRC] % 10 != Type[TYPE_DEST] % 10)
        pexit("Operation with two register of different size impossible\n");
    if ((Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX) &&
(Type[TYPE_SRC] % 10 > REGISTER_32b || Type[TYPE_DEST] % 10 > REGISTER_32b))
        pexit("Operation with a register unmanage by this program for the moment\n");
    if ((Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_SRC]) &&
(Type[TYPE_DEST] > TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_DEST]))
        pexit("Can mov from Reg offset only to regs of same size\n");
    if ((Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_DEST]) &&
(Type[TYPE_SRC] > TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_SRC]))
        pexit("Can mov to Reg offset only from regs of same size\n");

    /* OP with Regs Offset */
    if (Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_SRC]) {
        MOV_SRC_OFFREG_DEST_REG(Type, parser);
        return;
    }
    if (Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_DEST]) {
        MOV_SRC_REG_DEST_OFFREG(Type, parser);
        return;
    }
    /* SRC IS REG */
    if (Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX)
        MOV_SRC_REG_DEST_REG(Type, parser);
    if (Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && Type[TYPE_DEST] == TYPE_OP_IS_OFF)
        MOV_SRC_REG_DEST_OFF(Type, parser, idx);

    /* SRC IS IMM */
    if (Type[TYPE_SRC] == TYPE_OP_IS_IMM && Type[TYPE_DEST] == TYPE_OP_IS_OFF)
        MOV_SRC_IMM_DEST_OFF(parser, idx);
    if (Type[TYPE_SRC] == TYPE_OP_IS_IMM && Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX)
        MOV_SRC_IMM_DEST_REG(Type, parser, idx);

    /* SRC IS OFF */
    if (Type[TYPE_SRC] == TYPE_OP_IS_OFF && Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX)
        MOV_SRC_OFF_DEST_REG(Type, parser, idx);
}
