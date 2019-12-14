#include "builtin.h"
#include "ElfHelper.h"
#include "parser.h"

/* write elf header */
void Write_Basic_ElfHdr(int wr)
{
    uint64 hdr = MAGIC_ELF;
    write(wr, &hdr, 0x4);

    hdr = 0x1; // 32 b
    write(wr, &hdr, 0x1);

    hdr = 0x1; // little endian
    write(wr, &hdr, 0x1);

    hdr = 0x1; // original & current elf version
    write(wr, &hdr, 0x1);

    hdr = 0x0; // ABI target
    write(wr, &hdr, 0x1);

    hdr = 0x0; // depend ABI target
    write(wr, &hdr, 0x1);

    hdr = 0x0; // padding 7 bytes
    write(wr, &hdr, 0x7);

    hdr = 0x1; // repositionnable file (.o)
    write(wr, &hdr, 0x2);

    hdr = 0x3; // target machine (intel 80386)
    write(wr, &hdr, 0x2);

    hdr = 0x1; // actual version
    write(wr, &hdr, 0x4);
    /* end "Magic Elf" */
}

void Write_AfterProcess_ElfHdr(parser_t *parser, int wr)
{
    /* start program structure information (also header) */
    uint32 hdr;

    hdr = 0x0; // entry point address (virt addr start == 0x0)
    write(wr, &hdr, 0x4);

    hdr = 0x0; // start of program headers (no program header)
    write(wr, &hdr, 0x4);

    hdr = FIRST_ENTRY_POINT; // start of section headers (normally start at 0x40 -> based on header size)
    write(wr, &hdr, 0x4);

    hdr = 0x0; // flags (unuse on i386 target machine architecture)
    write(wr, &hdr, 0x4);

    hdr = SIZEOF_BASIC_HEADER; // sizeof header (this header /!\)
    write(wr, &hdr, 0x2);

    hdr = 0x0; // sizeof program header
    write(wr, &hdr, 0x2);

    hdr = 0x0; // number entries in program header
    write(wr, &hdr, 0x2);

    hdr = SIZEOF_STRUCT_SECTION_HEADER; // sizeof section header
    write(wr, &hdr, 0x2);

    hdr = SectionEntriesCalculator(parser, SECTION_SHDR_V); // number entries in section header
    write(wr, &hdr, 0x2);

    hdr = StrTableIndexFinder(parser->sec, SECTION_SHSTRTAB); // section headers string table (index in section header)
    write(wr, &hdr, 0x2);

    hdr = 0x0;
    for (uint i = SIZEOF_BASIC_HEADER; i < FIRST_ENTRY_POINT; i += 2)
        write(wr, &hdr, 0x2);
}
