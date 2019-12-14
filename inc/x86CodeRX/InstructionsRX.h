#ifndef _INSTRUCTION_RX_H_
#define _INSTRUCTION_RX_H_

#include "builtin.h"

/* x86 INSTR FNCT PTR USE */
void EXEC_MOV(parser_t *parser, uint idx);
void EXEC_ADD(parser_t *parser, uint idx);
void EXEC_SYSCALL(parser_t *parser, uint idx);
void EXEC_INT(parser_t *parser, uint idx);
void EXEC_NOP(parser_t *parser, uint idx);

void EXEC_JMP(parser_t *parser, uint idx);
void EXEC_JE(parser_t *parser, uint idx);
void EXEC_JNE(parser_t *parser, uint idx);
void EXEC_JB(parser_t *parser, uint idx);
void EXEC_JBE(parser_t *parser, uint idx);
void EXEC_JL(parser_t *parser, uint idx);
void EXEC_JLE(parser_t *parser, uint idx);

void EXEC_INC(parser_t *parser, uint idx);
void EXEC_DEC(parser_t *parser, uint idx);
void EXEC_CMP(parser_t *parser, uint idx);

void EXEC_CALL(parser_t *parser, uint idx);
void EXEC_RET(parser_t *parser, uint idx);

#endif
