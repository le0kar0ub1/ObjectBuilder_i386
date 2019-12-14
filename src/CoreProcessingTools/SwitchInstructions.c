#include "builtin.h"
#include "opcode.h"

uint8 *SwitchInstr(parser_t *parser, uint idx, uint8 *Type)
{
    uint8 tmp = Type[TYPE_DEST];
    Type[TYPE_DEST] = Type[TYPE_SRC];
    Type[TYPE_SRC] = tmp;

    tmp = parser->control->offreg[TYPE_SRC];
    parser->control->offreg[TYPE_SRC] = parser->control->offreg[TYPE_DEST];
    parser->control->offreg[TYPE_DEST] = tmp;

    tmp = parser->control->OffSize[TYPE_SRC];
    parser->control->OffSize[TYPE_SRC] = parser->control->OffSize[TYPE_DEST];
    parser->control->OffSize[TYPE_DEST] = tmp;

    char *stmp = parser->inp[idx][SRC];
    parser->inp[idx][SRC] = parser->inp[idx][DEST];
    parser->inp[idx][DEST] = stmp;
    return (Type);
}
