#include "builtin.h"

bool Overflow8(uint32 nbr)
{
    if (nbr > 0xff)
        return (false);
    return (true);
}

bool Overflow16(uint32 nbr)
{
    if (nbr > 0xffff)
        return (false);
    return (true);
}
