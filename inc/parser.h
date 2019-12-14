#ifndef _PARSER_H_
#define _PARSER_H_

/* PERSONNAL HEADERS */
#include "cmp_macro.h"
#include "lib.h"
#include "types_shortcut.h"
#include "ElfHelper.h"

#define SECTION_TEXT     ".text"
#define SECTION_DATA     ".data"
#define SECTION_STRTAB   ".strtab"
#define SECTION_SYMTAB   ".symtab"
#define SECTION_SHSTRTAB ".shstrtab"
#define SECTION_RELOC    ".rel.text"

#define SECTION_RODATA ".rodata"
#define SECTION_BSS    ".bss"


#define UNDEFINED_SECTION -0x1
#define SECTION_TEXT_V   0x0
#define SECTION_DATA_V   0x1
#define SECTION_RODATA_V 0x2
#define SECTION_BSS_V    0x3

#define SECTION_STRTAB_V   0x20
#define SECTION_SYMTAB_V   0x22
#define SECTION_SHSTRTAB_V 0x23
#define SECTION_SHDR_V     0x24
#define SECTION_RELOC_V    0x25

/* used to know where a symbol is used and put his address later */
struct varused {
    char  *sym; // sym used
    uint8 i;    // num of idx in string of this block
    uint8 flag; // size? for jmp/mov/..? etc...
    struct varused *next;
};

#define SIZEOF_CODE_BLOCK_SIZE 0x78 /* 0x78 ByteCode per block */
struct code {
    uint8          *out;  /* code part by part */
    uint8          i;     /* used for code storage iteration  */
    struct varused *var;  /* used to inject address late when the symbol will be define */
    struct code    *next; /* ptr to next output block section */
};
#define SIZEOF_CODE sizeof(struct code) /* sizeof struct */

/* Used to control the instruction runtime */
struct control {
    uint8 *type;
    uint8 *OffSize; /* Data size when mov To/from offset */
    bool  *offreg;  /* true if offset is from a reg */
};

typedef struct parser {
    struct code     **out;    /* ptr on first each section linked chain block*/
    char            ***inp;   /* file is divide by text -> line ->  word */
    struct control  *control; /* struct managing current information on instruction */
    int             section;  /* section runtime */

    /* elf file and all that shit */
    SectionHeader_t     *shdr; /* shdr */
    SectionSymtab_t     *sym;  /* section symtab */
    SymtabStorer_t      *str;  /* section strtab */
    SectionStrtab_t     *sec;  /* section shstrtab */
    uint                off;   /* current wiriting offset in file */
    SectionStrtab_t     *var;  /* tmp storing symbol type */
    SectionRelocation_t *reloc;/* relocation part */
} parser_t;


typedef struct keyword {
    char *mnem[100];                     /* basicaly instructions mnemoniques */
    void (*fnct[100])(parser_t *, uint); /* array function, trigger when got the mnemonique */
} keyw_t;

#define MAX_SECTION_NUMBER 0x10

struct code *malloc_next_CodeBlock(struct code *data);
struct code *Init_Storing_Instr(parser_t *parser, uint SECTION, uint);

#endif
