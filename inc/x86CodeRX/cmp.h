#ifndef _CMP_H_
#define _CMP_H_

#define CMP_REGOFF8_IMM8_B      0x80
    #define CMP_OFF8_IMM8_B_SECOND 0x3D
#define CMP_REGOFFH_IMMH_WD     0x81
#define CMP_REGOFFH_IMMH_B      0x83
    #define CMP_OFFH_IMMH_B_SECOND 0x3D
#define CMP_REGOFF8_IMM_BASEREG 0x38

#define CMP_REG8_REG8 0x38
#define CMP_REGH_REGH 0x39

#define CMP_REG8_OFFREG8 0x3A
#define CMP_REGH_OFFREGH 0x3B

#define CMP_AL_IMM8   0x3C
#define CMP_AX_IMM16  0x3D
#define CMP_EAX_IMM32 0x3D

#endif
