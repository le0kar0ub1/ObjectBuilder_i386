#include "builtin.h"
#include "x86_opcode/opcode.h"
#include "math.h"
#include "SymbolDescriptor.h"
#include "string.h"

extern struct REGISTER_t REGISTER;

uint8 IS_A_REG(char *s)
{
    uint8 type = TYPE_OP_IS_UNDEFINED;

    for (uint k = 0x0; REGISTER.regs[k][0]; k++)
        for (uint kk = 0x0; REGISTER.regs[k][kk]; kk++) {
            if (my_strcmp(REGISTER.regs[k][kk], s))
                type = (k * 0xA) + kk; /* represent index in REGISTER */
        }
    return (type);
}

uint8 SymbolFlagUsing(char **s)
{
    if (my_strcmp(s[0], "jmp") || my_strcmp(s[0], "je") || my_strcmp(s[0], "jne") ||
my_strcmp(s[0], "jb") || my_strcmp(s[0], "jbe") || my_strcmp(s[0], "jl") ||
my_strcmp(s[0], "jle"))
        return (SYMBOL_FLAG_REL_8); // badly considering always realtive jmp offset 1byte
    if (my_strcmp(s[0], "cmp") || my_strcmp(s[0], "inc") || my_strcmp(s[0], "dec"))
        return (SYMBOL_FLAG_REL_32);
    if (my_strcmp(s[0], "call"))
        return (SYMBOL_FLAG_REL_32_TEXT);
    return (SYMBOL_FLAG_ABS);
}

char *RemoveBrackets(char *s)
{
    if (s[0] == '[' && s[len(s) - 1] == ']') {
        s[len(s) - 1] = 0x0;
        s++;
    }
    return (s);
}

/* destination :: source */
uint8 InstructionOperandType(char *s, bool *b)
{
    uint8 type = TYPE_OP_IS_UNDEFINED;

    *b = false;
    /* Check if there is registers in mov operation */
    type = IS_A_REG(s);

    if (type == TYPE_OP_IS_UNDEFINED &&
(type = IS_A_REG(RemoveBrackets(strdup(s)))) != TYPE_OP_IS_UNDEFINED)
        *b = true;
    /* Check if there is Offset */
    if (type == TYPE_OP_IS_UNDEFINED && s[0] == '[' && s[len(s) - 0x1] == ']' &&
(MultiBase_GetNbr(RemoveBrackets(strdup(s))) != FP_NAN || NanOrNull(RemoveBrackets(strdup(s)))))
        type = TYPE_OP_IS_OFF;

    /* Check if there is Immediate value */
    /* Use the MultiBase_GetNbr like a checker */
    if (type == TYPE_OP_IS_UNDEFINED && (MultiBase_GetNbr(s) != FP_NAN || NanOrNull(s)))
        type = TYPE_OP_IS_IMM;

    /* symbol part */
    if (type == TYPE_OP_IS_UNDEFINED && sis_alpha(s))
        type = TYPE_OP_IS_IMM_FROM_SYM;
    if (type == TYPE_OP_IS_UNDEFINED && s[0] == '[' && s[len(s) - 0x1] == ']' &&
sis_alpha(RemoveBrackets(strdup(s))))
        type = TYPE_OP_IS_OFF_FROM_SYM;

    /* If nothing is finded let's stop the processus */
    if (type == TYPE_OP_IS_UNDEFINED)
        pexit("Bad operand type\n");
    return (type);
}

uint8 *Instruction3OperandType(parser_t *parser, uint idx)
{
    parser->inp[idx] = OffsetSize_CheckAndRemove(parser->inp[idx], parser->control);
    uint8 *type = malloc(0x3);
    type[0x0] = InstructionOperandType(strdup(parser->inp[idx][1]), &(parser->control->offreg[0]));
    type[0x1] = InstructionOperandType(strdup(parser->inp[idx][2]), &(parser->control->offreg[1]));
    type[0x2] = InstructionOperandType(strdup(parser->inp[idx][3]), &(parser->control->offreg[2]));
    /* if symbols used, replace by a value and fill in later */
    for (uint8 i = 0x0; i < 0x3; i++)
        if (type[i] == TYPE_OP_IS_OFF_FROM_SYM || type[i] == TYPE_OP_IS_IMM_FROM_SYM) {
            if (TYPE_OP_IS_OFF_FROM_SYM)
                parser->inp[idx][i + 0x1] = RemoveBrackets(parser->inp[idx][i + 0x1]);
            StoreVarUsedStructure(Init_Storing_Instr(parser, SECTION_TEXT_V, 0x0),
                parser->inp[idx][i + 0x1], SymbolFlagUsing(parser->inp[idx]));
            RelocationWriter(parser, Init_Storing_Instr(parser, SECTION_TEXT_V, 0x0)->i + 0x1, R_386_32);
            parser->inp[idx][i + 0x1] = SymbolCodeUsing_Remap(parser->inp[idx][i + 0x1]);
            if (type[i] == TYPE_OP_IS_IMM_FROM_SYM)
                type[i] = TYPE_OP_IS_IMM;
            else
                type[i] = TYPE_OP_IS_OFF;
        }
    return (type);
}

uint8 *Instruction2OperandType(parser_t *parser, uint idx)
{
    parser->inp[idx] = OffsetSize_CheckAndRemove(parser->inp[idx], parser->control);
    uint8 *type = malloc(0x2);

    type[0x0] = InstructionOperandType(strdup(parser->inp[idx][1]), &(parser->control->offreg[0]));
    type[0x1] = InstructionOperandType(strdup(parser->inp[idx][2]), &(parser->control->offreg[1]));

    /* if symbols used, replace by a value and fill in later */
    for (uint8 i = 0x0; i < 0x2; i++)
        if (type[i] == TYPE_OP_IS_OFF_FROM_SYM || type[i] == TYPE_OP_IS_IMM_FROM_SYM) {
            if (TYPE_OP_IS_OFF_FROM_SYM)
                parser->inp[idx][i + 0x1] = RemoveBrackets(parser->inp[idx][i + 0x1]);
            StoreVarUsedStructure(Init_Storing_Instr(parser, SECTION_TEXT_V, 0x0),
                parser->inp[idx][i + 0x1], SymbolFlagUsing(parser->inp[idx]));
            if (SymbolFlagUsing(parser->inp[idx]) != SYMBOL_FLAG_REL_8)
                RelocationWriter(parser, Init_Storing_Instr(parser, SECTION_TEXT_V, 0x0)->i + 0x1, R_386_32);
            parser->inp[idx][i + 0x1] = SymbolCodeUsing_Remap(parser->inp[idx][i + 0x1]);
            if (type[i] == TYPE_OP_IS_IMM_FROM_SYM)
                type[i] = TYPE_OP_IS_IMM;
            else
                type[i] = TYPE_OP_IS_OFF;
        }
    return (type);
}

uint8 Instruction1OperandType(parser_t *parser, uint idx)
{
    parser->inp[idx] = OffsetSize_CheckAndRemove(parser->inp[idx], parser->control);
    uint8 type;
    type = InstructionOperandType(strdup(parser->inp[idx][0x1]), &(parser->control->offreg[0]));
    if (type == TYPE_OP_IS_OFF_FROM_SYM || type == TYPE_OP_IS_IMM_FROM_SYM) {
        if (TYPE_OP_IS_OFF_FROM_SYM)
            parser->inp[idx][0x1] = RemoveBrackets(parser->inp[idx][0x1]);
        StoreVarUsedStructure(Init_Storing_Instr(parser, SECTION_TEXT_V, 0x0),
            parser->inp[idx][0x1], SymbolFlagUsing(parser->inp[idx]));
        if (SymbolFlagUsing(parser->inp[idx]) != SYMBOL_FLAG_REL_8 &&
        SymbolFlagUsing(parser->inp[idx]) != SYMBOL_FLAG_REL_32_TEXT)
            RelocationWriter(parser, Init_Storing_Instr(parser, SECTION_TEXT_V, 0x0)->i + 0x1, R_386_32);
        parser->inp[idx][0x1] = SymbolCodeUsing_Remap(parser->inp[idx][0x1]);
    }
    return (type);
}
