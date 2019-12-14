/*
** EPITECH PROJECT, 2017
** my_strlen
** File description:
** strlen
*/

#include "lib.h"

int ulen(uint8 *data)
{
    int i = 0x0;

    for (; data[i]; i++);
    return (i);
}

int my_strlen(char const *str)
{
    int a = 0;

    for (; str[a]; a++);
    return (a);
}
