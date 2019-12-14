#include "lib.h"
#include "types_shortcut.h"
#include "cmp_macro.h"
#include "math.h"
#include "limits.h"

bool NanOrNull(char *s)
{
    if (my_strcmp("0x0", s))
        return (true);
    if (my_strcmp("0", s))
        return (true);
    if (my_strcmp("0b0", s))
        return (true);
    return (false);
}

uint32 HexaToDec(char *s)
{
    uint32 mod = 0x1;
    uint32 ret = 0x0;

    for (int k = len(s) - 0x1; k >= 0x0; k--, mod *= 0x10) {
        if ((s[k] < 0x30 || s[k] > 0x39) &&
    (s[k] < 0x41 || s[k] > 0x46) && (s[k] < 0x61 || s[k] > 0x66))
            return (FP_NAN);
        if (ret + (s[k] - 0x30) * mod > UINT_MAX - 1)
            pexit("Number Overflow maximum system size\n");
        if (s[k] >= 0x41 && s[k] <= 0x46)
            ret += (s[k] - 0x41 + 0xA) * mod;
        else if (s[k] >= 0x61 && s[k] <= 0x66)
            ret += (s[k] - 0x61 + 0xA) * mod;
        else
            ret += (s[k] - 0x30) * mod;
    }
    return (ret);
}

uint32 BinToDec(char *s)
{
    uint32 mod = 0x1;
    uint32 ret = 0x0;

    for (int k = len(s) - 0x1; k >= 0x0; k--, mod *= 0x2) {
        if (s[k] != 0x30 && s[k] != 0x31)
            return (FP_NAN);
        if (ret + (s[k] - 0x30) * mod > (UINT_MAX - 1))
            pexit("Number Overflow maximum system size\n");
        ret += (s[k] - 0x30) * mod;
    }
    return (ret);
}

uint32 DecToDec(char *s)
{
    uint32 mod = 0x1;
    uint32 ret = 0x0;

    for (int k = len(s) - 0x1; k >= 0x0; k--, mod *= 0xA) {
        if (s[k] < 0x30 && s[k] > 0x39)
            return (FP_NAN);
        if (ret + (s[k] - 0x30) * mod > (UINT_MAX - 1))
            pexit("Number Overflow maximum system size\n");
        ret += (s[k] - 0x30) * mod;
    }
    return (ret);
}

uint32 MultiBase_GetNbr(char *nbr)
{
    if (!nbr)
        return (FP_NAN);
    uint8 lenght = len(nbr);

    if (nbr[0] == '-')
        nbr++;
    if (lenght > 2 && my_strncmp(nbr, "0x", 0x2)) {
        nbr += 2;
        return (HexaToDec(nbr));
    }
    if (lenght > 2 && my_strncmp(nbr, "0b", 0x2)) {
        nbr += 2;
        return (BinToDec(nbr));
    }
    if (!sis_num(nbr))
        return (FP_NAN);
    return (DecToDec(nbr));
}
