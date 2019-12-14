#include "builtin.h"
#include "limits.h"
#include "math.h"

bool CheckSize8(char *data)
{
    uint32 test = MultiBase_GetNbr((char *)data);

    if (test > (UCHAR_MAX - 1) || (test == FP_NAN && !Neg32(test, data) && !NanOrNull(data)))
        return (false);
    return (true);
}

bool CheckSize16(char *data)
{
    uint32 test = MultiBase_GetNbr((char *)data);

    if (test > (USHRT_MAX - 1) || (test == FP_NAN && !Neg32(test, data) && !NanOrNull(data)))
        return (false);
    return (true);
}

bool CheckSize32(char *data)
{
    uint32 test = MultiBase_GetNbr((char *)data);

    if (test > (UINT_MAX - 1) || (test == FP_NAN && !Neg32(test, data) && !NanOrNull(data)))
        return (false);
    return (true);
}
