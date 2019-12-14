#include "builtin.h"

void EXEC_ADD(parser_t *parser, uint idx)
{
    sput("EXEC ADD");
    parser->control = NULL;
    idx += 1;
    return;
}
