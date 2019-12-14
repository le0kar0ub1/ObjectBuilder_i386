#include "builtin.h"
#include "math.h"

void DATA_Missing_Operand_Verification(parser_t *parser, uint idx)
{
    if (!(parser->inp[idx][1])) {
        sput("Missing Operand: ");
        my_put_tab(parser->inp[idx]);
        pexit(NULL);
    }
}

bool DATA_Declare_Initialized_Data(parser_t *parser, uint idx)
{
    if ((parser->inp[idx][1][0] == '\'' && parser->inp[idx][1][len(parser->inp[idx][1]) - 1] == '\'')
|| (parser->inp[idx][1][0] == '\"' && parser->inp[idx][1][len(parser->inp[idx][1]) - 1] == '\"')) {
        if (parser->inp[idx][2])
            pexit("parser error, too many argument when define initialize data\n");
        struct code *data = Init_Storing_Instr(parser, SECTION_DATA_V, len(parser->inp[idx][1]) - 2);
        for (uint i = 0x1; parser->inp[idx][1][i + 1]; i++, data->i += 0x1)
            data->out[data->i] = (parser->inp[idx][1][i] == 0x7F) ? 0x0 : parser->inp[idx][1][i];
        return (true);
    }
    return (false);
}

void EXEC_DEFINE_BYTE(parser_t *parser, uint idx)
{
    DATA_Missing_Operand_Verification(parser, idx);
    if (DATA_Declare_Initialized_Data(parser, idx))
        return;
    struct code *data = Init_Storing_Instr(parser, SECTION_DATA_V, 0x2);
    uint8 sz = Neg8((uint8)MultiBase_GetNbr(parser->inp[idx][1]), parser->inp[idx][1]);
    if (sz == FP_NAN && !NanOrNull(parser->inp[idx][1]))
        pexit("NAN argument when Define Byte\n");
    data->out[data->i] = sz;
    data->i += 0x1;
}

void EXEC_DEFINE_WORD(parser_t *parser, uint idx)
{
    DATA_Missing_Operand_Verification(parser, idx);
    if (DATA_Declare_Initialized_Data(parser, idx))
        return;
    struct code *data = Init_Storing_Instr(parser, SECTION_DATA_V, 0x3);
    uint16 sz = Neg16((uint16)MultiBase_GetNbr(parser->inp[idx][1]), parser->inp[idx][1]);
    if (sz == FP_NAN && !NanOrNull(parser->inp[idx][1]))
        pexit("NAN argument when Define Word\n");
    uint16 DIVISOR = 0x1000;
    for (int8 i = 0x1; i > -0x1; i--) {
        data->out[data->i + i] = sz / (DIVISOR);
        sz -= data->out[data->i + i] * (DIVISOR);
        DIVISOR /= 0x100;
    }
    data->i += 0x2;
}

void EXEC_DEFINE_DBLW(parser_t *parser, uint idx)
{
    DATA_Missing_Operand_Verification(parser, idx);
    if (DATA_Declare_Initialized_Data(parser, idx))
        return;
    struct code *data = Init_Storing_Instr(parser, SECTION_DATA_V, 0x5);
    uint32 sz = Neg32(MultiBase_GetNbr(parser->inp[idx][1]), parser->inp[idx][1]);
    if (sz == FP_NAN && !NanOrNull(parser->inp[idx][1]))
        pexit("NAN argument when Define Double Word\n");
    /* see the method, got it? */
    uint32 DIVISOR = 0x1000000;
    for (int8 i = 0x3; i > -0x1; i--) {
        data->out[data->i + i] = sz / (DIVISOR);
        sz -= data->out[data->i + i] * (DIVISOR);
        DIVISOR /= 0x100;
    }
    data->i += 0x4;
}
