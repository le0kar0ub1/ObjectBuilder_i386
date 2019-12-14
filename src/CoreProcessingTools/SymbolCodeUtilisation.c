#include "builtin.h"

/* this will make a new tab replacing symbol by 0xffffffff */
char *SymbolCodeUsing_Remap(char *s)
{
    s = malloc(0xB);

    s[0x0] = '0';
    s[0x1] = 'x';
    for (uint i = 0x2; i < 0xA; i++)
        s[i] = 'f';
    s[0xA] = 0x0;
    return (s);
}
