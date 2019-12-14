#ifndef OPERATIONS_MIXING_H_
#define OPERATIONS_MIXING_H_

/* THIS HEADER DEFINE A PREFIX FOR OPERATION MIXING 16 & 32 */

#define OPERAND_SIZE_PREFIX 0x66
#define ADDRESS_SIZE_PREFIX 0x67

/*
NO PREFIX:           mov 32b from a 32b regs to 32b address
OPERAND_SIZE_PREFIX: mov 16b from a 16b regs to 32b address
ADDRESS_SIZE_PREFIX: mov 32b from a 32b regs to 16b address
OPERAND_SIZE_PREFIX & ADDRESS_SIZE_PREFIX: mov 16b from a 16b regs to a 16b address
*/

#define b16br16to32addr   OPERAND_SIZE_PREFIX
#define b32br32to16addr   ADDRESS_SIZE_PREFIX
#define b16br16to16addr_1 OPERAND_SIZE_PREFIX
#define b16br16to16addr_2 ADDRESS_SIZE_PREFIX

#endif
