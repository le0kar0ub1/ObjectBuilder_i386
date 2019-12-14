#include "builtin.h"
#include "SymbolDescriptor.h"

void SetSymbol_AsGlobal(parser_t *parser, uint idx)
{
    if (len_tab(parser->inp[idx]) > 0x2)
        pexit("parser error when global declared");
    if (parser->var == NULL)
        InitStrTab(&(parser->var), parser->inp[idx][1], STB_GLOBAL);
    else
        StructureStrtabStore(parser->var, parser->inp[idx][1], STB_GLOBAL, SYMBOL_SECTION_NO_CARE);
}

bool symbol_ParamDef(parser_t *parser, uint idx)
{
    if (my_strcmp(parser->inp[idx][0x0], "global")) {
        SetSymbol_AsGlobal(parser, idx);
        return (true);
    }
    if (my_strcmp(parser->inp[idx][0x0], "extern")) {
        SetSymbol_AsGlobal(parser, idx);
        RelocationWriter(parser, Init_Storing_Instr(parser, SECTION_TEXT_V, 0x0)->i + 0x1, R_386_PC32 + 0x400);
        SymtabStorerStore(parser, parser->inp[idx][1], STT_NOTYPE, parser->section, 0x0);
        return (true);
    }
    return (false);
}
