#include "builtin.h"
#include "ElfHelper.h"
#include "string.h"
#include "SymbolDescriptor.h"

void HexPut(uint8 n)
{
    uint8 ch = 0x0, save = n;
    int max = 0x0;

    sput("0x");
    for (; n / 0x10 > 0; n /= 0x10, max++);
    for (; max >= 0; max--) {
        ch = n % 0x10;
        if (ch > 0x9)
            my_putchar(0x41 + (ch - 0xA));
        else
            my_putchar(ch + 0x30);
        if (max == 0x0 || max == 0x1)
            n = save;
        else
            n = save / power(max, 0x10);
    }
}

void DumpSection(struct code *ptr, int fd)
{
    uint sz = 0x0;

    struct code *data = ptr;
    for (; data; data = data->next) {
        write(fd, data->out, data->i);
        sz += data->i;
    }
    if (sz % 0x10 != 0x0)
        WriteNullByte(fd, 0x10 - (sz % 0x10));
}

void DumpStrtab(SectionStrtab_t *d, int fd)
{
    SectionStrtab_t *data = d;
    uint sz = 0x0;

    for (; data; data = data->next) {
        write(fd, data->name, len(data->name) + 0x1);
        sz += len(data->name) + 1;
    }
    if (sz % 0x10 != 0x0)
        WriteNullByte(fd, 0x10 - (sz % 0x10));
}

void DumpSymtabStorer(SymtabStorer_t *sym, int fd)
{
    SymtabStorer_t *data = sym;
    uint sz = 0x0;

    for (; data; data = data->next) {
        write(fd, data->name, len(data->name) + 0x1);
        sz += len(data->name) + 1;
    }
    if (sz % 0x10 != 0x0)
        WriteNullByte(fd, 0x10 - (sz % 0x10));
}

void DumpSymtab(SectionSymtab_t *ptr, int fd)
{
    SectionSymtab_t *data = ptr;
    uint sz = 0x0;

    for (; data; data = data->next) {
        write(fd, data, SIZEOF_STRUCT_SECTION_SYMTAB);
        sz += SIZEOF_STRUCT_SECTION_SYMTAB;
    }
}

void DumpShdr(parser_t *parser, int fd)
{
    SectionHeader_t *data = parser->shdr;
    uint sz = 0x0;

    for (; data; data = data->next) {
        write(fd, data, SIZEOF_STRUCT_SECTION_HEADER);
        sz += SIZEOF_STRUCT_SECTION_HEADER;
    }
    if (sz % 0x10 != 0x0)
        WriteNullByte(fd, 0x10 - (sz % 0x10));
}

void DumpSectionRelocation(SectionRelocation_t *r, int fd)
{
    SectionRelocation_t *rel = r;
    uint sz = 0x0;

    for (; rel; rel = rel->next) {
        write(fd, rel, 0x8);
        sz += 0x8;
    }
    if (sz % 0x10 != 0x0)
        WriteNullByte(fd, 0x10 - (sz % 0x10));
}

void WriteNullByte(int fd, uint sz)
{
    uint8 wr = 0x0;
    for (; sz > 0x0; sz--)
        write(fd, &wr, 0x1);
}


void UpdateAndInit(parser_t *parser)
{
    /* init strtab sect with null section */
    InitStrTab(&parser->sec, strdup("\0"), SYMBOL_IS_NULL_SECTION);
    /* store used code section */
    StructureStrtabStore(parser->sec, strdup(SECTION_TEXT), STT_SECTION, SYMBOL_SECTION_NO_CARE);
    if (parser->out[SECTION_DATA_V])
        StructureStrtabStore(parser->sec, strdup(SECTION_DATA), STT_SECTION, SYMBOL_SECTION_NO_CARE);
    /* init now symbol tab & section headers :) */
    InitSymbolTab(parser);
    InitShdr(parser);

    /* Replace in code The symbols used by their respective address */
    ReplaceSymByAddr(parser);
}

void dump_data(parser_t *parser, int fd)
{
    /* complete header elf */
    Write_Basic_ElfHdr(fd);
    Write_AfterProcess_ElfHdr(parser, fd);

    /* write shdr in file */
    DumpShdr(parser, fd);

    /* write code section */
    DumpSection(parser->out[SECTION_TEXT_V], fd);
    if (parser->out[SECTION_DATA_V])
        DumpSection(parser->out[SECTION_DATA_V], fd);

    /* dump section shstrtab*/
    DumpStrtab(parser->sec, fd);

    /* dump section symtab*/
    DumpSymtab(parser->sym, fd);

    /* dump section strtab */
    DumpSymtabStorer(parser->str, fd);

    /* dump section relocation */
    if (parser->reloc)
        DumpSectionRelocation(parser->reloc, fd);

    // DUMPSymtabStorerCHAR(parser->str);
    // DUMPstrtabCHAR(parser->sec);
    // DUMPstrtabCHAR(parser->var);
}
