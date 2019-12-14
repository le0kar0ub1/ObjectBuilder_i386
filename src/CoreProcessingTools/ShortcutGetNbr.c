#include "builtin.h"
#include "opcode.h"
#include "math.h"

/* Maximal reduce for the GetNbr to produce clean code */
/* Give right at these function to exit() if overflow detected */

uint8 GetNbr8(char *ev)
{
    if (!CheckSize8(ev)) {
        print("%s Overflow\n", ev);
        exit(0x54);
    }
    uint8 sz = Neg8((uint8)MultiBase_GetNbr(ev), ev);
    if (sz == FP_NAN && !NanOrNull(ev))
        pexit("NAN argument when get number\n");
    return (sz);
}

uint16 GetNbr16(char *ev)
{
    if (!CheckSize16(ev)) {
        print("%s Overflow\n", ev);
        exit(0x54);
    }
    uint16 sz = Neg16((uint16)MultiBase_GetNbr(ev), ev);
    if (sz == FP_NAN && !NanOrNull(ev))
        pexit("NAN argument when get number\n");
    return (sz);
}

uint32 GetNbr32(char *ev)
{
    uint32 sz = Neg32((uint32)MultiBase_GetNbr(ev), ev);
    if (sz == FP_NAN && !NanOrNull(ev))
        pexit("NAN argument when get number\n");
    return (sz);
}

bool VGetNbr8(char *ev)
{
    if (!CheckSize8(ev))
        return (false);
    uint8 sz = Neg8((uint8)MultiBase_GetNbr(ev), ev);
    if (sz == FP_NAN && !NanOrNull(ev))
        return (false);
    return (true);
}

bool VGetNbr16(char *ev)
{
    if (!CheckSize16(ev))
        return (false);
    uint16 sz = Neg16((uint16)MultiBase_GetNbr(ev), ev);
    if (sz == FP_NAN && !NanOrNull(ev))
        return (false);
    return (true);
}

bool VGetNbr32(char *ev)
{
    uint32 sz = Neg32((uint32)MultiBase_GetNbr(ev), ev);
    if (sz == FP_NAN && !NanOrNull(ev))
        return (false);
    return (true);
}
