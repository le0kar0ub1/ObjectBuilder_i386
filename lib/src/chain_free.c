/*
** EPITECH PROJECT, 2018
** chain_free.c
** File description:
** chain_free.c
*/

#include "lib.h"

void chain_free(char const *nfree, ...)
{
    int nbr = matoi(nfree);
    va_list ap;

    if (nbr < 0)
        return;
    va_start(ap, nfree);
    for (; nbr > 0; nbr--)
        free(va_arg(ap, char *));
    va_end(ap);
}
