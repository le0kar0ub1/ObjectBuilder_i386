#include "builtin.h"
#include "SymbolDescriptor.h"
#include "limits.h"
#include "jmp.h"

SectionSymtab_t *GiveSymtabStructure(parser_t *parser, char *name)
{
    uint IdxInStrab = StrTabIndexFinder((SectionStrtab_t *)parser->str, name);

    for (SectionSymtab_t *sym = parser->sym; sym; sym = sym->next)
        if (sym->st_name == IdxInStrab)
            return (sym);
    pexit("Symbol Used in code section isn't exist\n");
    return (NULL);
}

uint IdxRelocStart(uint8 i, uint8 *data, uint8 sz)
{
    uint8 count = 0x0;

    for (; data[i]; i++) {
        if (data[i] == 0xff)
            count++;
        else
            count = 0x0;
        if (count == sz)
            break;
    }
    if (count != sz)
        pexit("Invalid relocation target\n");
    return (i - (sz - 0x1));
}

void ReplaceSymByAddr(parser_t *parser)
{
    for (uint sec = 0x0;parser->out[sec]; sec++)
        for (struct code *data = parser->out[sec]; data; data = data->next)
            for (struct varused *var = data->var; var; var = var->next) {
                SectionSymtab_t *sym = GiveSymtabStructure(parser, var->sym);
                if (var->flag == SYMBOL_FLAG_ABS) {
                    uint idx = IdxRelocStart(var->i, data->out, 0x4);
                    data->out = S32Dumper(sym->st_value, idx, data->out);
                } else if (var->flag == SYMBOL_FLAG_REL_8) {
                    uint idx = IdxRelocStart(var->i, data->out, 0x1);
                    if (data->out[idx - 1] == OPCODE_JMP_REL8 && ((sym->st_value < idx + 0x1 &&
                0x100000000 - NegOp32(sym->st_value, idx + 0x1) > UCHAR_MAX) ||
                (sym->st_value > idx + 0x1 && sym->st_value - (idx + 0x1) > UCHAR_MAX)))
                        pexit("Only jump of size +/- 128 in memory are allowed");
                    data->out[idx] = NegOp8(sym->st_value, idx + 0x1);

                } else if (var->flag == SYMBOL_FLAG_REL_32) {
                    /* considering only symbol from other sections than .text */
                    uint idx = IdxRelocStart(var->i, data->out, 0x4);
                    data->out = S32Dumper(sym->st_value, idx, data->out);
                } else if (var->flag == SYMBOL_FLAG_REL_32_TEXT) {
                    /* relative in .text */
                    uint idx = IdxRelocStart(var->i, data->out, 0x4);
                    if (sym->st_value == 0x0)
                        data->out = S32Dumper(NegOp32(sym->st_value, idx + 0x3), idx, data->out);
                    else
                        data->out = S32Dumper(NegOp32(sym->st_value, idx + 0x4), idx, data->out);
                }
            }
}

bool LastRelocationOffset(parser_t *parser)
{
    SectionRelocation_t *rel = parser->reloc;
    struct code *code = parser->out[SECTION_TEXT_V];

    if (!rel || !code)
        return (0x0);
    for (; code->next; code = code->next);
    for (; rel->next; rel = rel->next);
    if (code->i == rel->r_offset - 0x1)
        return (true);
    return (false);
}

/* relocation writer */
void RelocationWriter(parser_t *parser, uint off, uint info)
{
    if (parser->reloc == NULL) {
        parser->reloc = malloc(sizeof(SectionRelocation_t));
        parser->reloc->r_offset = off;
        parser->reloc->r_info = info + 0x300;
        parser->reloc->next = NULL;
    } else {
        SectionRelocation_t *rel = parser->reloc;
        for (; rel->next; rel = rel->next);
        rel->next = malloc(sizeof(SectionRelocation_t));
        rel->next->r_offset = off;
        rel->next->r_info = info + 0x300;
        rel->next->next = NULL;
    }
}
