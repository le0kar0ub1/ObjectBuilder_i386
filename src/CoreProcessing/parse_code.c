#include "builtin.h"
#include "parser.h"
#include "x86_opcode/opcode.h"
#include "string.h"

// http://sparksandflames.com/files/x86InstructionChart.html

// ref.x86asm.net/coder64.html

// https://gist.github.com/mikesmullin/6259449

/* Offset Intel Syntax size */
char *OffsetOpSize[7] = {"byte", "word", "dword", "BYTE", "WORD", "DWORD", NULL};

/* x86 mnemonique and function managing for each instruction allowed */
keyw_t key[] =
{
    {{"mov"  , "add"   , "int"   , "sysenter"  , "nop"   , "jmp"   ,
    "je"    , "jne"   , "jb"   , "jbe"   , "jl"   , "jle"     ,
    "inc"    , "dec"   , "cmp"  , "call"    , "ret"   , NULL},
    {EXEC_MOV, EXEC_ADD, EXEC_INT, EXEC_SYSCALL, EXEC_NOP, EXEC_JMP,
     EXEC_JE, EXEC_JNE, EXEC_JB, EXEC_JBE, EXEC_JL, EXEC_JLE,
     EXEC_INC, EXEC_DEC, EXEC_CMP, EXEC_CALL, EXEC_RET}}, /* Section Text */

    {{"db"           , "dw"            , "dd"              ,
      "DB"           , "DW"            , "DD"              , NULL},
    {EXEC_DEFINE_BYTE, EXEC_DEFINE_WORD, EXEC_DEFINE_DBLW,
     EXEC_DEFINE_BYTE, EXEC_DEFINE_WORD, EXEC_DEFINE_DBLW}}, /* Section Data   */

    {{"db", "dw", "dd", "DB", "DW", "DD", NULL},
    {NULL}}, /* Section Rodata */

    {{"resb", NULL},
    {NULL}}  /* Section BSS    */
};

bool IsInstructionExist(char *s, uint8 section)
{
    for (int instr = 0x0; key[section].mnem[instr]; instr++)
        if (my_strcmp(key[section].mnem[instr], s))
            return (true);
    return (false);
}

void StoreVarUsedStructure(struct code *code, char *name, uint8 flag)
{
    if (code->var == NULL) {
        code->var = malloc(sizeof(struct varused));
        code->var->sym = name;
        code->var->i = code->i;
        code->var->flag = flag;
        code->var->next = NULL;
    } else {
        struct varused *var = code->var;
        for (; var->next; var = var->next);
        var->next = malloc(sizeof(struct varused));
        var = var->next;
        var->sym = name;
        var->i = code->i;
        var->flag = flag;
        var->next = NULL;
    }
}

void running_code(parser_t *parser, uint idx)
{
    int instr = 0x0;
    int skip = 0x0;

    if (parser->section < 0x0)
        pexit("No section declared\n");
    /* reset control structure */
    // memset(parser->control, 0x0, sizeof(struct control));
    /* if a symbol is declared here */
    if (parser->section == SECTION_DATA_V &&
len_tab(parser->inp[idx]) > 0x2 && IsInstructionExist(TOLOWER(parser->inp[idx][0x1]), SECTION_DATA_V) &&
!IsInstructionExist(parser->inp[idx][0x0], SECTION_DATA_V)) {
        SymtabStorerStore(parser, parser->inp[idx][0x0],
        STT_NOTYPE, SECTION_DATA_V, SectionSizeCalculator(parser, parser->section));
        skip = 0x1;
        (parser->inp[idx]) += skip; // skip the symbol declared
    }
    for (; key[parser->section].mnem[instr]; instr++)
        if (my_strcmp(key[parser->section].mnem[instr], parser->inp[idx][0x0]))
            break;
    if (key[parser->section].mnem[instr] == NULL) {
        print("%s: ", parser->inp[idx][0x0]);
        pexit("Unknow instruction\n");
    }
    key[parser->section].fnct[instr](parser, idx); /* JMP on the function managing the instruction */
    (parser->inp[idx]) -= skip; // pop the symbol at last
}
