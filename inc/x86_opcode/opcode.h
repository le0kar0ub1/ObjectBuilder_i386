#ifndef OPCODE_H_
#define OPCODE_H_

/* https://pdos.csail.mit.edu/6.828/2005/readings/i386/ */

#include "types_shortcut.h"

extern struct REGISTER_t REGISTER;

/* OPCODE x86 definition (1 to 15 bytes)*/

/* Prefixes: (optionnal) */
/* Direction: (1 bit) -> 1 == register is Destination, 0 == register is Source */
/* Operation size: (1 bit) -> 1 == word, 0 == byte */
/* Opcode: (6 bits) instruction family code */
/* MOD: (2 bits) register mode (displacement) */
/* Register: (3 bits) register identifier */
/* R/M: (3 bits) register/Memory operand */

struct REGISTER_t {
    char  *regs[9][5];
    uint8 val[8];
};

struct REGISTER_MEMORY_t {
    uint8 val[9];
};

struct MOD_t {
    uint8 val[5];
};

#define RegsToRegsBaseValue    0xC0 /* + Reg.val(dest) + Reg.val(src) * 8 */
#define RegsOffToRegsBaseValue 0x0  /* + Reg.val(dest) + Reg.val(src) * 8 */
#define OffToRegsBaseValue     0xD  /* + (Reg.val(dest) / 10) * 8 */

/* define Value for each Offset size controller */
#define BYTE_VALUE        0x1
#define WORD_VALUE        0x2
#define DBWD_VALUE        0x3
#define UNDEFINED_OFF_VAL 0x10

/* index in REGISTER_t in opcode.c */
#define REGISTER_8b  0x0
#define REGISTER_16b 0x1
#define REGISTER_32b 0x2

/* index value in parser->inp */
#define DEST 0x1
#define SRC  0x2

/* Macro helper for instruction executing */
#define TYPE_OP_IS_REG_MAX      100 /* (0 >= x < 120) direct send by index descriptor */

#define TYPE_OP_IS_IMM_FROM_SYM 110
#define TYPE_OP_IS_OFF_FROM_SYM 111

#define TYPE_OP_IS_IMM          121
#define TYPE_OP_IS_OFF          122
#define TYPE_OP_IS_OFF_FROM_REG 123
#define TYPE_OP_IS_UNDEFINED    124

#define TYPE_SRC  0x1
#define TYPE_DEST 0x0

static inline uint8 RegsOpcodeCalculator(uint8 *type, uint8 base)
{
    return (base +
            (REGISTER.val[type[TYPE_SRC] / 10] * 8) +
            (REGISTER.val[type[TYPE_DEST] / 10]));
}

#endif
