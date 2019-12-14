#ifndef MOV_H_
#define MOV_H_

/* MOV INSTRUCTION STRUCTURE */
    /* Mov Var type (1 byte) (define below) */
    /*  */

/* destination_source */
enum x86_MOV {
    AL_OFF8     = 0xA0,
    AX_OFFH	    = 0xA1,
    EAX_OFFH    = 0xA1,
    OFF8_AL     = 0xA2,
    OFFH_AX	    = 0xA3,
    OFFH_EAX    = 0xA3,

    OFF8_R8   = 0x88,
    R8_R8     = 0x88,
    OFFH_RH   = 0x89,
    RH_RH     = 0x89,
    R8_OFF8   = 0x8A,
    RH_OFFH   = 0x8B,


    R8_IMM8 = 0xB0, // + reg val
    RH_IMMH = 0xB8, // + reg val

    OFF8_IMM8 = 0xC6,
    OFFH_IMMH = 0xC7,
};
/* R   == register      */
/* OFF == offset        */
/* IMM == direct value  */
/* H   == high          */

#endif
