#include "builtin.h"
#include "string.h"

extern char *OffsetOpSize[4];

bool AreBracketArround(char *s)
{
    if (s[0] == '[' && s[len(s) - 1] == ']')
        return (true);
    return (false);
}

char **OffsetSize_CheckAndRemove(char **inp, struct control *control)
{
    char **new = malloc(len_tab(inp) * 0x8 + 0x8);
    uint8 Offinc = 0x0;
    control->OffSize = malloc(0x5);
    memset(control->OffSize, 0x10, 0x5);

    new[0] = inp[0];
    uint8 k = 0x1, i = 0x1, j = 0x0;

    for (; inp[i]; i++) {
        for (j = 0x0; OffsetOpSize[j]; j++) {
            if (my_strcmp(OffsetOpSize[j], inp[i])) {
                control->OffSize[Offinc] = (j > 2) ? (j - 2) : (j + 1);
                break;
            }
        }
        if (!(OffsetOpSize[j])) {
            Offinc++;
            new[k] = inp[i];
            k++;
        }
    }
    control->OffSize[Offinc] = 0x0;
    new[k] = NULL;
    return (new);
}
