#include "builtin.h"
#include "ElfHelper.h"
#include "string.h"
#include "SymbolDescriptor.h"

void Write_NullSectionSym(SectionSymtab_t *sym)
{
    SectionSymtab_t *data = sym;
    memset(data, 0x0, SIZEOF_STRUCT_SECTION_SYMTAB);
    data->next = NULL;
}

void StructureSymtabStore(parser_t *parser,
uint32 name, uint32 value, uint32 size, uchar info, uint32 other, uint16 shndx)
{
    SectionSymtab_t *data = parser->sym;

    for (; data->next; data = data->next);
    data->next = malloc(sizeof(SectionSymtab_t));
    data = data->next;
    data->next = NULL;
    data->st_name = name;   // index in string table
    data->st_value = value; // associate Value symbol
    data->st_size = size;   // associated size, often 0
    data->st_info = info;   // symbol type and attrib
    data->st_other = other;
    data->st_shndx = shndx; // section header table index
}

uint GiveLabelBind(parser_t *parser, SymtabStorer_t *pr)
{
    uint bind = STB_LOCAL;
    SectionStrtab_t *d = parser->var;
    SymtabStorer_t  *p = pr;

    for (; d; d = d->next)
        if (my_strcmp(d->name, p->name))
            bind = d->flags;
    return (ELF32_ST_INFO(bind, STT_NOTYPE));
}

char *AssignedSection(int8 sec)
{
    switch (sec) {
        case SECTION_TEXT_V:
            return (SECTION_TEXT);
            break;
        case SECTION_DATA_V:
            return (SECTION_DATA);
            break;
        case UNDEFINED_SECTION:
            return (NULL);
            break;
    }
    pexit("Symbol have invalid assigned section\n");
    return (NULL);
}

void InitSymbolTab(parser_t *parser)
{
    parser->sym = malloc(sizeof(SectionSymtab_t));
    /* NULL #INIT */
    Write_NullSectionSym(parser->sym);

    /* file */
    StructureSymtabStore(parser, 0x1, 0x0, 0x0, ELF32_ST_INFO(STB_LOCAL, STT_FILE),
        STV_DEFAULT, SHN_ABS);

    /* code section */
    StructureSymtabStore(parser, 0x0, 0x0, 0x0, ELF32_ST_INFO(STB_LOCAL, STT_SECTION),
        STV_DEFAULT, StrTableIndexFinder(parser->sec, SECTION_TEXT));
    if (parser->out[SECTION_DATA_V])
        StructureSymtabStore(parser, 0x0, 0x0, 0x0, ELF32_ST_INFO(STB_LOCAL, STT_SECTION),
            STV_DEFAULT, StrTableIndexFinder(parser->sec, SECTION_DATA));

    /* local Symbols */
    SymtabStorer_t *data = parser->str->next;
    for (uint sz = 0x1; data; data = data->next) {
        if (data->flags != STT_FILE
        && GiveLabelBind(parser, data) != ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE))
            StructureSymtabStore(parser, sz, data->memory, 0x0,
                ELF32_ST_INFO(STB_LOCAL, STT_NOTYPE),
                STV_DEFAULT, StrTableIndexFinder(parser->sec, AssignedSection(data->section)));
        sz += len(data->name) + 0x1;
    }

    /* Global Symbols */
    data = parser->str->next;
    for (uint sz = 0x1; data; data = data->next) {
        if (data->flags != STT_FILE
        && GiveLabelBind(parser, data) != ELF32_ST_INFO(STB_LOCAL, STT_NOTYPE))
            StructureSymtabStore(parser, sz, data->memory, 0x0,
                ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE),
                STV_DEFAULT, StrTableIndexFinder(parser->sec, AssignedSection(data->section)));
        sz += len(data->name) + 0x1;
    }
}
