#ifndef _ELF_PROCESSING_H_
#define _ELF_PROCESSING_H_

void Write_AfterProcess_ElfHdr(parser_t *parser, int wr);;
void WriteSectionDescription(uint32 *data);
void Write_NullSectionHdr(SectionHeader_t *);
void Write_NullSectionSym(SectionSymtab_t *);
void Write_Basic_ElfHdr(int wr);
void StoreVarUsedStructure(struct code *code, char *name, uint8 flag);
uint SymtabStorerEntriesCalculator(parser_t *parser);
uint SymtabStorerIndexFinder(parser_t *parser, char *cmp);
uint SymtabStorer_StrIndexFinder(parser_t *parser, char *name);
void SymtabStorerStore(parser_t *parser, char *name, uint8 flag, uint8 section, uint32 memory);
void InitSymtabStorer(parser_t *parser, char *name, uint8 flag, uint8 section, uint32 memory);
void StructureSymtabStore(parser_t *parser, uint32 name, uint32 value, uint32 size, uchar info, uint32 other, uint16 shndx);
void NewSymbol(parser_t *parser, char *s, uint8);
void InitSymbolTab(parser_t *parser);
void StructureShdrStore(parser_t *parser, uint32 name, uint32 type, uint32 flags, uint32 addr, uint32 offset, uint32 size, uint32 link, uint32 info, uint32 addralign, uint32 entsize);
void NewSection(parser_t *parser, char *s, uint8);
void InitShdr(parser_t *parser);
void StructureStrtabStore(SectionStrtab_t *old, char *name, uint8 flag, uint8);
void InitStrTab(SectionStrtab_t **, char *rd, uint8 flag);
void OffsetCalculatorInit(parser_t *parser);
uint32 OffsetCalculator(parser_t *parser, uint size);
void AssignSectionOffset(parser_t *parser);
void WriteNullByte(int fd, uint sz);

#endif
