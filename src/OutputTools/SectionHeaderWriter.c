#include "string.h"
#include "builtin.h"
#include "ElfHelper.h"
#include "SymbolDescriptor.h"

void Write_NullSectionHdr(SectionHeader_t *hdr)
{
    SectionHeader_t *data = hdr;
    memset(data, 0x0, SIZEOF_STRUCT_SECTION_HEADER);
    data->sh_type = SHT_NULL;
    data->next = NULL;
}

uint8 FirstGlobal_SymbolIndex(SectionSymtab_t *sym)
{
    SectionSymtab_t *data = sym;
    uint i = 0x0;

    for (; data; data = data->next, i++)
        if (data->st_info == ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE))
            return (i);
    return (0xFF);
}

/* view section header struct */
void StructureShdrStore(parser_t *parser, uint32 name, uint32 type, uint32 flags, uint32 addr,
uint32 offset, uint32 size, uint32 link, uint32 info, uint32 addralign, uint32 entsize)
{
    SectionHeader_t *data = parser->shdr;

    for (; data->next; data = data->next);
    data->next = malloc(sizeof(SectionHeader_t));
    data = data->next;
    data->next = NULL;
    data->sh_name = name;           // index in strtab section
    data->sh_type = type;           // section content semantic
    data->sh_flags = flags;         // section memory type (wr...)
    data->sh_addr = addr;           // if memory appear in memory process, else 0
    data->sh_offset = offset;       // offset of this section from the beginning of the file
    data->sh_size = size;           // sizeof this section
    data->sh_link = link;           // section header index link
    data->sh_info = info;           // extra information depending from section
    data->sh_addralign = addralign; // address alignement constraint
    data->sh_entsize = entsize;     // if fixed entries in the section, else 0
}

void InitShdr(parser_t *parser)
{
    parser->shdr = malloc(sizeof(SectionHeader_t));
    uint sz;

    /* store strtab string */
    StructureStrtabStore(parser->sec, strdup(SECTION_SHSTRTAB), STT_SECTION, SYMBOL_SECTION_NO_CARE);
    StructureStrtabStore(parser->sec, strdup(SECTION_SYMTAB), STT_SECTION, SYMBOL_SECTION_NO_CARE);
    StructureStrtabStore(parser->sec, strdup(SECTION_STRTAB), STT_SECTION, SYMBOL_SECTION_NO_CARE);
    if (parser->reloc)
        StructureStrtabStore(parser->sec, strdup(SECTION_RELOC), STT_SECTION, SYMBOL_SECTION_NO_CARE);
    /* NULL #INIT */
    Write_NullSectionHdr(parser->shdr);
    /* text */
    sz =  SectionSizeCalculator(parser, SECTION_TEXT_V);
    StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_TEXT), SHT_PROGBITS,
        SHF_EXECINSTR | SHF_ALLOC, 0x0, 0x0, sz, 0x0, 0x0, 0x10, 0x0);
    /* data */
    if (parser->out[SECTION_DATA_V]) {
        sz = SectionSizeCalculator(parser, SECTION_DATA_V);
        StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_DATA), SHT_PROGBITS,
            SHF_WRITE | SHF_ALLOC, 0x0, 0x0, sz, 0x0, 0x0, 0x4, 0x0);
    }
    /* shstrtab */
    sz = SectionSizeCalculator(parser, SECTION_SHSTRTAB_V);
    StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_SHSTRTAB), SHT_STRTAB,
        0x0, 0x0, 0x0, sz, 0x0, 0x0, 0x1, 0x0);
    /* symtab */
    sz = SectionSizeCalculator(parser, SECTION_SYMTAB_V);
    StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_SYMTAB), SHT_SYMTAB,
        0x0, 0x0, 0x0, sz, StrTableIndexFinder(parser->sec, SECTION_STRTAB),
        FirstGlobal_SymbolIndex(parser->sym), 0x4, 0x10);
    /* strtab */
    sz = SectionSizeCalculator(parser, SECTION_STRTAB_V);
    StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_STRTAB), SHT_STRTAB,
        0x0, 0x0, 0x0, sz, 0x0, 0x0, 0x1, 0x0);

    /* relocation .text */
    if (parser->reloc) {
        sz = SectionEntriesCalculator(parser, SECTION_RELOC_V) * 0x8;
        StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_RELOC), SHT_REL,
            0x0, 0x0, 0x0, sz, StrTableIndexFinder(parser->sec, SECTION_SYMTAB),
            0x1, 0x4, 0x8);
    }

    OffsetCalculatorInit(parser);
    for (SectionHeader_t *p = parser->shdr; p; p = p->next)
        if (p->sh_type != SHT_NULL)
            p->sh_offset = OffsetCalculator(parser, p->sh_size);
}

// void InitShdr(parser_t *parser)
// {
//     parser->shdr = malloc(sizeof(SectionHeader_t));
//     uint sz;
//
//     StructureStrtabStore(parser->sec, strdup(SECTION_SHSTRTAB), SYMBOL_IS_ORG_SECTION);
//     StructureStrtabStore(parser->sec, strdup(SECTION_SYMTAB), SYMBOL_IS_ORG_SECTION);
//     StructureStrtabStore(parser->sec, strdup(SECTION_STRTAB), SYMBOL_IS_ORG_SECTION);
//     /* NULL */
//     StructureShdrStore(parser, 0x0, SHT_NULL, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
//     Write_NullSectionHdr(parser->shdr);
//     /* text */
//     sz =  SectionSizeCalculator(parser, SECTION_TEXT_V);
//     StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_TEXT), SHT_PROGBITS,
//         SHF_EXECINSTR | SHF_ALLOC, 0x0, OffsetCalculator(parser, sz), sz, 0x0,
//         0x0, 0x10, 0x0);
//     /* data */
//     if (parser->out[SECTION_DATA_V]) {
//         sz = SectionSizeCalculator(parser, SECTION_TEXT_V);
//         StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_DATA), SHT_PROGBITS,
//             SHF_WRITE | SHF_ALLOC, 0x0, OffsetCalculator(parser, sz), sz, 0x0,
//             0x0, 0x4, 0x0);
//     }
//     /* shstrtab */
//     sz = SectionSizeCalculator(parser, SECTION_SHSTRTAB_V);
//     StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_SHSTRTAB), SHT_STRTAB,
//         0x0, 0x0, OffsetCalculator(parser, sz), sz, 0x0, 0x0, 0x1, 0x0);
//     /* symtab */
//     sz = SectionSizeCalculator(parser, SECTION_SYMTAB_V);
//     sz = 0x40;
//     StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_SYMTAB), SHT_SYMTAB,
//         0x0, 0x0, OffsetCalculator(parser, sz), sz,
//         SectionEntriesCalculator(parser, SECTION_SYMTAB_V), 0x3, 0x4, 0x10);
//     /* strtab */
//     sz = SectionSizeCalculator(parser, SECTION_STRTAB_V) + 1;
//     StructureShdrStore(parser, StrTabIndexFinder(parser->sec, SECTION_STRTAB), SHT_STRTAB,
//         0x0, 0x0, OffsetCalculator(parser, sz), sz, 0x0, 0x0, 0x1, 0x0);
// }
