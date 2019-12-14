#include "builtin.h"
#include "opcode.h"
#include "string.h"
#include "operations_mixing.h"
#include "cmp.h"

extern struct REGISTER_t REGISTER;

void EXEC_CMP_DEST_REG_SRC_IMM(parser_t *parser, uint idx, uint8 *type)
{
    uint8 *op = malloc(0x4);
    uint8 k = 0x0;

    uint8 reg = type[TYPE_DEST] % 10;
    uint8 regval = type[TYPE_DEST] / 10;
    if (reg == REGISTER_8b) {
        if (!VGetNbr8(parser->inp[idx][SRC]))
            pexit("Invalid Number comparaison");
        if (regval == 0x0) {
            op[k] = CMP_AL_IMM8;
            op[k + 1] = GetNbr8(parser->inp[idx][SRC]);
            k += 2;
        } else {
            op[k] = CMP_REGOFF8_IMM8_B;
            op[k + 1] = 0xf8 + regval;
            op[k + 2] = GetNbr8(parser->inp[idx][SRC]);
            k += 3;
        }
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    } else {
        if (reg == REGISTER_16b) {
            if (!VGetNbr16(parser->inp[idx][SRC]))
                pexit("Invalid Number comparaison");
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        if (regval == 0x0 && !VGetNbr16(parser->inp[idx][SRC]) && reg == REGISTER_32b) {
            op[k] = CMP_EAX_IMM32;
            k += 1;
            op = NumberDumperS32(parser->inp[idx][SRC], k, op);
            k += 4;
        } else if (regval == 0x0 && !VGetNbr8(parser->inp[idx][SRC]) && reg == REGISTER_16b) {
                op[k] = CMP_EAX_IMM32;
                k += 1;
                op = NumberDumperS32(parser->inp[idx][SRC], k, op);
                k += 2;
        } else {
            if (VGetNbr8(parser->inp[idx][SRC]))
                op[k] = CMP_REGOFFH_IMMH_B;
            else
                op[k] = CMP_REGOFFH_IMMH_WD;
            op[k + 1] = 0xf8 + regval;
            k += 2;
            uint8 tmp;
            if (VGetNbr8(parser->inp[idx][SRC])) {
                tmp = 0x1;
            } else if (VGetNbr16(parser->inp[idx][SRC])) {
                tmp = 0x2;
            } else {
                tmp = 0x4;
            }
            op = NumberDumperS32(parser->inp[idx][SRC], k, op);
            k += tmp;
        }
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_CMP_DEST_REG_SRC_REG(parser_t *parser, uint8 *type)
{
    uint8 *op = malloc(0x4);
    uint8 k = 0x0;

    if (type[TYPE_SRC] % 10 != type[TYPE_DEST] % 10)
        pexit("Comparaison with register different size\n");
    uint8 reg = type[TYPE_SRC] % 10;
    if (reg == REGISTER_8b) {
        op[k] = CMP_REG8_REG8;
        op[k + 1] = RegsOpcodeCalculator(type, RegsToRegsBaseValue);
        WriteOpcode(parser, SECTION_TEXT_V, op, 0x2);
    } else {
        if (reg == REGISTER_16b) {
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        op[k] = CMP_REGH_REGH;
        op[k + 1] = RegsOpcodeCalculator(type, RegsToRegsBaseValue);
        k += 2;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_CMP_DEST_OFFREG_SRC_IMM(parser_t *parser, uint idx, uint8 *type)
{
    uint8 *op = malloc(0x8);
    uint8 k = 0x0;

    if (parser->control->OffSize[TYPE_DEST] == UNDEFINED_OFF_VAL)
        pexit("Sizeof of mov from/to offset unspecified\n");
    if (type[TYPE_DEST] % 10 != REGISTER_32b)
        pexit("Invalid Offset size specification\n");
    parser->inp[idx][DEST] = RemoveBrackets(parser->inp[idx][DEST]); // we already know the instr type

    if (parser->control->OffSize[TYPE_DEST] == BYTE_VALUE) {
        if (!VGetNbr8(parser->inp[idx][SRC]))
            pexit("Invalid Immediate value\n");
        op[k] = CMP_REGOFF8_IMM8_B;
        op[k + 1] = CMP_REGOFF8_IMM_BASEREG + (type[TYPE_DEST] / 10);
        op[k + 2] = GetNbr8(parser->inp[idx][SRC]);
        k += 3;
        WriteOpcode(parser, SECTION_TEXT_V, op, 0x3);
    } else {
        if (parser->control->OffSize[TYPE_DEST] == WORD_VALUE) {
            if (!VGetNbr16(parser->inp[idx][SRC]))
                pexit("Invalid Immediate value\n");
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        uint8 tmp;
        if (VGetNbr8(parser->inp[idx][SRC])) {
            tmp = 0x1;
            op[k] = CMP_REGOFFH_IMMH_B;
        } else if (VGetNbr16(parser->inp[idx][SRC])) {
            tmp = 0x2;
            op[k] = CMP_REGOFFH_IMMH_WD;
        } else {
            tmp = 0x4;
            op[k] = CMP_REGOFFH_IMMH_WD;
        }
        op[k + 1] = CMP_REGOFF8_IMM_BASEREG + (type[TYPE_DEST] / 10);
        k += 2;
        op = NumberDumperS32(parser->inp[idx][SRC], k, op);
        k += tmp;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_CMP_DEST_OFF_SRC_IMM(parser_t *parser, uint idx, uint8 *type)
{
    uint8 *op = malloc(0xA);
    uint8 k = 0x0;

    if (parser->control->OffSize[TYPE_DEST] == UNDEFINED_OFF_VAL)
        pexit("Sizeof of mov from/to offset unspecified\n");
    if (type[TYPE_DEST] % 10 != REGISTER_32b)
        pexit("Invalid Offset size specification\n");
    parser->inp[idx][DEST] = RemoveBrackets(parser->inp[idx][DEST]);

    if (parser->control->OffSize[TYPE_DEST] == BYTE_VALUE) {
        if (!VGetNbr8(parser->inp[idx][SRC]))
            pexit("Invalid Immediate value\n");
        op[k] = CMP_REGOFF8_IMM8_B;
        op[k + 1] = CMP_OFF8_IMM8_B_SECOND;
        k += 2;
        op = NumberDumperS32(parser->inp[idx][DEST], k, op);
        k += 4;
        op[k] = GetNbr8(parser->inp[idx][SRC]);
        k += 1;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    } else {
        if (parser->control->OffSize[TYPE_DEST] == WORD_VALUE) {
            if (!VGetNbr16(parser->inp[idx][SRC]))
                pexit("Invalid Immediate value\n");
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        uint8 tmp;
        if (VGetNbr8(parser->inp[idx][SRC])) {
            tmp = 0x1;
            op[k] = CMP_REGOFFH_IMMH_B;
        } else if (VGetNbr16(parser->inp[idx][SRC])) {
            tmp = 0x2;
            op[k] = CMP_REGOFFH_IMMH_WD;
        } else {
            tmp = 0x4;
            op[k] = CMP_REGOFFH_IMMH_WD;
        }
        op[k + 1] = CMP_OFFH_IMMH_B_SECOND;
        k += 2;
        op = NumberDumperS32(parser->inp[idx][DEST], k, op);
        k += 4;
        op = NumberDumperS32(parser->inp[idx][SRC], k, op);
        k += tmp;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_CMP_DEST_REG_SRC_OFFREG(parser_t *parser, uint idx, uint8 *type)
{
    uint8 *op = malloc(0x4);
    uint8 k = 0x0, tmp = 0x0;

    if (type[TYPE_DEST] % 10 != parser->control->OffSize[TYPE_SRC] - 1)
        pexit("Invalid operands size\n");
    parser->inp[idx][SRC] = RemoveBrackets(parser->inp[idx][SRC]);

    if (type[TYPE_DEST] == REGISTER_8b) {
        op[k] = CMP_REG8_OFFREG8;
        tmp = type[TYPE_SRC];
        type[TYPE_SRC] = type[TYPE_DEST];
        type[TYPE_DEST] = tmp;
        op[k + 1] = RegsOpcodeCalculator(type, 0x0);
        k += 2;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    } else {
        if (type[TYPE_DEST] == REGISTER_16b) {
            op[k] = OPERAND_SIZE_PREFIX;
            k++;
        }
        op[k] = CMP_REGH_OFFREGH;
        tmp = type[TYPE_SRC];
        type[TYPE_SRC] = type[TYPE_DEST];
        type[TYPE_DEST] = tmp;
        op[k + 1] = RegsOpcodeCalculator(type, 0x0);
        k += 0x2;
        WriteOpcode(parser, SECTION_TEXT_V, op, k);
    }
}

void EXEC_CMP(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) < 0x3 || len_tab(parser->inp[idx]) > 0x4)
        pexit("Invalid argument number for Cmp, need 2 operands\n");
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
Type[TYPE_SRC] % 10 != Type[TYPE_DEST] % 10 &&
!parser->control->offreg[TYPE_DEST] && !parser->control->offreg[TYPE_SRC])
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

    if (Type[TYPE_SRC] == TYPE_OP_IS_IMM && Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_DEST]) {
        EXEC_CMP_DEST_OFFREG_SRC_IMM(parser, idx, Type);
    } else if (Type[TYPE_DEST] == TYPE_OP_IS_IMM && Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_SRC]) {
        /* switching */
        Type = SwitchInstr(parser, idx, Type);
        EXEC_CMP_DEST_OFFREG_SRC_IMM(parser, idx, Type);
        Type = SwitchInstr(parser, idx, Type);
    } else if (Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX &&
    !parser->control->offreg[TYPE_DEST] && !parser->control->offreg[TYPE_SRC]) {
        EXEC_CMP_DEST_REG_SRC_REG(parser, Type);
    } else if (Type[TYPE_SRC] == TYPE_OP_IS_IMM && Type[TYPE_DEST] == TYPE_OP_IS_OFF) {
        EXEC_CMP_DEST_OFF_SRC_IMM(parser, idx, Type);
    } else if (Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX && !parser->control->offreg[TYPE_DEST] &&
    Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_SRC]) {
        EXEC_CMP_DEST_REG_SRC_OFFREG(parser, idx, Type);
    } else if (Type[TYPE_SRC] < TYPE_OP_IS_REG_MAX && !parser->control->offreg[TYPE_SRC] &&
    Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX && parser->control->offreg[TYPE_DEST]) {
        Type = SwitchInstr(parser, idx, Type);
        EXEC_CMP_DEST_REG_SRC_OFFREG(parser, idx, Type);
        Type = SwitchInstr(parser, idx, Type);
    } else if (Type[TYPE_DEST] < TYPE_OP_IS_REG_MAX && !parser->control->offreg[TYPE_DEST] &&
    Type[TYPE_SRC] == TYPE_OP_IS_IMM) {
        EXEC_CMP_DEST_REG_SRC_IMM(parser, idx, Type);
    } else {
        tab(parser->inp[idx]);
        pexit(" :Unkow parser error occured\n");
    }
}
