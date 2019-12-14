#ifndef _BUILTIN_H_
#define _BUILTIN_H_

/* PERSONNAL HEADERS */
#include "cmp_macro.h"
#include "lib.h"
#include "parser.h"
#include "InstructionsWR.h"
#include "InstructionsRX.h"
#include "ProcessingHelper.h"
#include "ElfProcessing.h"

#define PRINT_FILE for (int ii = 0; parser->inp[ii]; ii++) { my_put_tab(parser->inp[ii]); retl; } exit(0);

bool LastRelocationOffset(parser_t *parser);
bool checkIf_SymOffUsed(char *s);
void ReplaceSymByAddr(parser_t *parser);
bool symbol_ParamDef(parser_t *parser, uint idx);
void dump_data(parser_t *parser, int fd);
void modify_running_label(parser_t *parser, uint idx);
uint modify_running_section(parser_t *parser, uint idx);
void running_code(parser_t *parser, uint);
void section_header(parser_t *parser);
char *cut_total_path(char *s);
void start_parser(char *rd);
void get_symbols(parser_t *parser);
char ***get_file(int rd, char *file);
void FillIn_ShStrTab(parser_t *parser);
void Parser_Initialisation(parser_t *parser, int fdrd, char *rd);
void prog_exit(char *ERROR, char *file);
void Init_CodeSection(parser_t *parser, uint8 sect);
void UpdateAndInit(parser_t *parser);
char *SymbolCodeUsing_Remap(char *s);
void RelocationWriter(parser_t *parser, uint off, uint info);

#endif
