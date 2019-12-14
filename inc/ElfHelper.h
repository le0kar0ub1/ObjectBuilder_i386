#ifndef ELF_H_
#define ELF_H_

/* PERSONNAL HEADERS */
#include "cmp_macro.h"
#include "types_shortcut.h"
#include "elf.h"

#define MAGIC_ELF 0x464C457F

#define FIRST_ENTRY_POINT   0x40
#define SIZEOF_BASIC_HEADER 0x34

typedef struct SectionHeader {
    uint32   sh_name;
    uint32   sh_type;
    uint32   sh_flags;
    uint32   sh_addr;
    uint32   sh_offset;
    uint32   sh_size;
    uint32   sh_link;
    uint32   sh_info;
    uint32   sh_addralign;
    uint32   sh_entsize;

    struct SectionHeader *next;
} SectionHeader_t;
#define SIZEOF_SECTION_HEADER sizeof(SectionHeader_t)
#define SIZEOF_STRUCT_SECTION_HEADER (sizeof(SectionHeader_t) - 0x8)

typedef struct SectionSymtab {
    uint32   st_name;
    uint32   st_value;
    uint32   st_size;
    uchar    st_info;
    uchar    st_other;
    uint16   st_shndx;

    struct SectionSymtab *next;
} SectionSymtab_t;

#define SIZEOF_STRUCT_SECTION_SYMTAB (sizeof(SectionSymtab_t) - 0x8)

typedef struct SectionStrtab {
    char   *name;
    uint8  flags;   // type assigned
    uint8  section; // section assigned
    struct SectionStrtab *next;
} SectionStrtab_t;

#define SIZEOF_STRUCT_SECTION_STRTAB (sizeof(SectionStrtab_t))

typedef struct SymtabStorer {
    char   *name;
    uint8  flags;   // type assigned
    uint8  section; // section assigned
    uint32 memory;  // place in memory (if necessary)
    struct SymtabStorer *next;
} SymtabStorer_t;

#define SIZEOF_STRUCT_SYMTAB_STORER (sizeof(SectionStrtab_t))

/* relocations */
typedef struct Relocation_t {
    uint32 r_offset;
    uint32 r_info;
    struct Relocation_t * next;
} SectionRelocation_t;

typedef struct RelocationAddend_t {
    uint32 r_offset;
    uint32 r_info;
    int32  r_addend;
} SectionRelocationAddend_t;

#endif
