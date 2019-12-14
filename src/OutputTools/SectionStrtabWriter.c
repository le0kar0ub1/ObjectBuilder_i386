#include "builtin.h"
#include "ElfHelper.h"
#include "SymbolDescriptor.h"

void StructureStrtabStore(SectionStrtab_t *old, char *name, uint8 flag, uint8 section)
{
    SectionStrtab_t *data = old;

    for (; (data)->next; (data) = (data)->next);
    (data)->next = malloc(SIZEOF_STRUCT_SECTION_STRTAB);
    (data)->next->next = NULL;
    (data)->next->name = name;
    (data)->next->flags = flag;
    (data)->next->section = section;
}

void InitStrTab(SectionStrtab_t **str, char *rd, uint8 flag)
{
    *str = malloc(sizeof(SectionStrtab_t));
    (*str)->next = NULL;
    (*str)->name = rd;
    (*str)->flags = flag;
    (*str)->section = SYMBOL_SECTION_NO_CARE;
}
