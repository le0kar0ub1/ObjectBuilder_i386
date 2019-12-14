#include "builtin.h"
#include "x86_opcode/opcode.h"

struct REGISTER_t REGISTER =
{
    .regs = {
                {"al", "ax", "eax", "ds", NULL},
                {"cl", "cx", "ecx", "es", NULL},
                {"dl", "dx", "edx", "fs", NULL},
                {"bl", "bx", "ebx", "gs", NULL},
                {"ah", "sp", "esp", "ss", NULL},
                {"ch", "bp", "ebp", "cs", NULL},
                {"dh", "si", "esi", "ip", NULL},
                {"bh", "di", "edi", NULL, NULL},
                {NULL}
            },
    .val = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7},
};

struct REGISTER_MEMORY_t REGISTER_MEMORY =
{
    .val = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7},
    // 0: (BX) + (SI) + displacement (0, 1 or 2 bytes long)
    // 1: (BX) + (DI) + displacement (0, 1 or 2 bytes long)
    // 2: (BP) + (SI) + displacement (0, 1 or 2 bytes long)
    // 3: (BP) + (DI) + displacement (0, 1 or 2 bytes long)
    // 4: (SI) + displacement (0, 1 or 2 bytes long)
    // 5: (DI) + displacement (0, 1 or 2 bytes long)
    // 6: (BP) + displacement unless mod = 00 (see mod table)
    // 7: (BX) + displacement (0, 1 or 2 bytes long)
};

struct MOD_t MOD =
{
    .val = {0x0, 0x1, 0x2, 0x3},
    // 0: If r/m is 110, Displacement (16 bits) is address; otherwise, no displacement
    // 1: Eight-bit displacement, sign-extended to 16 bits
    // 2: 16-bit displacement (example: MOV [BX + SI]+ displacement,al)
    // 3: r/m is treated as a second "reg" field
};
