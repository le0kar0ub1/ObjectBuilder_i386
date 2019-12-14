#include "builtin.h"
#include "x86_opcode/opcode.h"

/* Alloc new CodeBlock*/
struct code *malloc_next_CodeBlock(struct code *data)
{
    data->next = malloc(SIZEOF_CODE);
    data = data->next;
    data->out = malloc(SIZEOF_CODE_BLOCK_SIZE);
    data->i = 0x0;
    data->var = NULL;
    data->next = NULL;
    return (data);
}

/* Find the current CodeBlock and Alloc a new if needs */
struct code *Init_Storing_Instr(parser_t *parser, uint SECTION, uint size)
{
    struct code *data = (struct code *)parser->out[SECTION];

    for (uint i = 0x0; data && data->next; i++)
        data = data->next;
    if (data->i >= SIZEOF_CODE_BLOCK_SIZE - size)
        data = malloc_next_CodeBlock(data);
    return (data);
}

void WriteOpcode(parser_t *parser, uint8 section, uint8 *data, uint8 sz)
{
    struct code *wr = Init_Storing_Instr(parser, section, sz + 0x1);
    for (uint8 i = 0x0; i < sz; i++) {
        wr->out[wr->i] = data[i];
        wr->i += 1;
    }
}
