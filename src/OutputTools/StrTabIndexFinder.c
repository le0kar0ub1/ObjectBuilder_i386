#include "builtin.h"

uint StrTabIndexFinder(SectionStrtab_t *old, char *name)
{
    SectionStrtab_t *data = old;
    uint sz = 0x1;

    if (!name)
        return (0x0);
    for (uint i = 0x0; data; data = data->next, i++) {
        if (my_strcmp(data->name, name))
            return (sz - 0x1);
        sz += len(data->name) + 0x1;
    }
    print("%s: ", name);
    pexit("Invalid Symbol or Section in Section .strtab\n");
    return (0x0);
}

uint StrTableIndexFinder(SectionStrtab_t *old, char *name)
{
    SectionStrtab_t *data = old;

    if (!name)
        return (0x0);
    for (uint i = 0x0; data; data = data->next, i++)
        if (my_strcmp(data->name, name))
            return (i);
    print("%s: ", name);
    pexit("Invalid Symbol or Section in Section .strtab\n");
    return (0x0);
}
