#ifndef JMP_H_
#define JMP_H_

#define OPCODE_JMP_REGS_BASE 0xE0

#define OPCODE_JMP_REL8 0xEB
#define OPCODE_JMP_RELH 0xE9

#define OPCODE_JMP_ABS8 0xFF
#define OPCODE_JMP_ABSH 0xFF

/* Describe all i386 jmp in memory depend of carry flag */
#define JE8   0x74
#define JEH1  0x0F
#define JEH2  0x84

#define JNE8  0x75
#define JNEH1 0x0F
#define JNEH2 0X85

#define JB8   0x72
#define JBH1  0x0F
#define JBH2  0X82

#define JBE8  0x76
#define JBEH1 0x0F
#define JBEH2 0x86

#define JL8   0x7C
#define JLH1  0x0F
#define JLH2  0x8C

#define JLE8  0x7E
#define JLEH1 0x0F
#define JLEH2 0x8E

// enum x86_CONDITIONAL_JMP {
//     JE8 = 0x74,
//     JEH = 0x0F84,
//
//     JNE8 = 0x75,
//     JNEH = 0x0F85,
//
//     JB8 = 0x72,
//     JBH = 0x0F82,
//
//     JBE8 = 0x76,
//     JBEH = 0x0F86,
//
//     JL8 = 0x7C,
//     JLH = 0x0F8C,
//
//     JLE8 = 0x7E,
//     JLEH = 0x0F8E,
// };

#endif
