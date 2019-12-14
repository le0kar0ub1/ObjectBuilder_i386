#include "builtin.h"

uint8 *NumberDumperS16_Stop0(char *n, uint8 k, uint8 *op)
{
    uint16 DIVISOR = 0x1000;
    uint16 sz = GetNbr16(n);
    for (int8 i = k + 1; i > k - 1 && sz / DIVISOR > 0x0; i--) {
        op[i] = sz / DIVISOR;
        sz -= op[i] * DIVISOR;
        DIVISOR /= 0x100;
    }
    return (op);
}

uint8 *NumberDumperS32_Stop0(char *n, uint8 k, uint8 *op)
{
    uint32 DIVISOR = 0x1000000;
    uint32 sz = GetNbr32(n);
    for (int8 i = k + 3; i > k - 1 && sz / DIVISOR > 0x0; i--) {
        op[i] = sz / DIVISOR;
        sz -= op[i] * DIVISOR;
        DIVISOR /= 0x100;
    }
    return (op);
}

uint8 *NumberDumperS16(char *n, uint8 k, uint8 *op)
{
    uint16 DIVISOR = 0x1000;
    uint16 sz = GetNbr16(n);
    for (int8 i = k + 1; i > k - 1; i--) {
        op[i] = sz / DIVISOR;
        sz -= op[i] * DIVISOR;
        DIVISOR /= 0x100;
    }
    return (op);
}

uint8 *NumberDumperS32(char *n, uint8 k, uint8 *op)
{
    uint32 DIVISOR = 0x1000000;
    uint32 sz = GetNbr32(n);
    for (int8 i = k + 3; i > k - 1; i--) {
        op[i] = sz / DIVISOR;
        sz -= op[i] * DIVISOR;
        DIVISOR /= 0x100;
    }
    return (op);
}

uint8 *S8Dumper(uint8 sz, uint8 k, uint8 *op)
{
    op[k + 1] = sz;
    return (op);
}

uint8 *S16Dumper(uint16 sz, uint8 k, uint8 *op)
{
    uint32 DIVISOR = 0x1000000;
    for (int8 i = k + 1; i > k - 1; i--) {
        op[i] = sz / DIVISOR;
        sz -= op[i] * DIVISOR;
        DIVISOR /= 0x100;
    }
    return (op);
}

uint8 *S32Dumper(uint32 sz, uint8 k, uint8 *op)
{
    uint32 DIVISOR = 0x1000000;
    for (int8 i = k + 3; i > k - 1; i--) {
        op[i] = sz / DIVISOR;
        sz -= op[i] * DIVISOR;
        DIVISOR /= 0x100;
    }
    return (op);
}
