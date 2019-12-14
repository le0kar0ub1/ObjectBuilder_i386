#ifndef _PROCESSING_HELPER_H_
#define _PROCESSING_HELPER_H_

/* Code Helper definition */
uint8 *SwitchInstr(parser_t *parser, uint idx, uint8 *Type);
uint SectionSizeCalculator(parser_t *parser, uint8 section);
uint StrTableIndexFinder(SectionStrtab_t *old, char *name);
uint StrTabIndexFinder(SectionStrtab_t *, char *name);
uint SectionEntriesCalculator(parser_t *parser, uint8 section);
char **OffsetSize_CheckAndRemove(char **inp, struct control *control);
uint8 IS_A_REG(char *s);
char *RemoveBrackets(char *s);
uint8 *S8Dumper(uint8 sz, uint8 k, uint8 *op);
uint8 *S16Dumper(uint16 sz, uint8 k, uint8 *op);
uint8 *S32Dumper(uint32 sz, uint8 k, uint8 *op);
uint8 *NumberDumperS16_Stop0(char *n, uint8 k, uint8 *op);
uint8 *NumberDumperS16_Stop0(char *n, uint8 k, uint8 *op);
uint8 *NumberDumperS16(char *, uint8 k, uint8 *op);
uint8 *NumberDumperS32(char *, uint8 k, uint8 *op);
bool VGetNbr8(char *ev);
bool VGetNbr16(char *ev);
bool VGetNbr32(char *ev);
uint8  GetNbr8(char *);
uint16 GetNbr16(char *);
uint32 GetNbr32(char *);
bool CheckSize8(char *data);
bool CheckSize16(char *data);
bool CheckSize32(char *data);
bool REG_OP_REG(uint8 *type);
void WriteOpcode(parser_t *parser, uint8 section, uint8 *data, uint8 sz);
uint8 InstructionOperandType(char *s, bool *);
uint8 *Instruction3OperandType(parser_t *parser, uint idx);
uint8 *Instruction2OperandType(parser_t *parser, uint idx);
uint8 Instruction1OperandType(parser_t *parser, uint idx);
bool NanOrNull(char *s);
uint32 MultiBase_GetNbr(char *nbr);
uint8 NegOp8(uint8 f, uint8 s);
uint16 NegOp16(uint16 f, uint16 s);
uint32 NegOp32(uint32 f, uint32 s);
uint8  Neg8(uint8 nbr, char *n);
uint16 Neg16(uint16 nbr, char *n);
uint32 Neg32(uint32 nbr, char *n);
bool Overflow8(uint32 nbr);
bool Overflow16(uint32 nbr);

#endif
