#include "builtin.h"
#include "math.h"

uint8 NegOp8(uint8 f, uint8 s)
{
    if (f < s)
        return (0x100 - (s - f));
    return (f - s);
}

uint16 NegOp16(uint16 f, uint16 s)
{
    if (f < s)
        return (0x10000 - (s - f));
    return (f - s);
}

uint32 NegOp32(uint32 f, uint32 s)
{
    if (f < s)
        return (0x100000000 - (s - f));
    return (f - s);
}

uint8 Neg8(uint8 nbr, char *n)
{
    if (nbr == FP_NAN)
        return (FP_NAN);
    if (n[0] == '-')
        return (0x100 - nbr);
    return (nbr);
}

uint16 Neg16(uint16 nbr, char *n)
{
    if (nbr == FP_NAN)
        return (FP_NAN);
    if (n[0] == '-')
        return (0x10000 - nbr);
    return (nbr);
}

uint32 Neg32(uint32 nbr, char *n)
{
    if (nbr == FP_NAN)
        return (FP_NAN);
    if (n[0] == '-')
        return (0x100000000 - nbr);
    return (nbr);
}
