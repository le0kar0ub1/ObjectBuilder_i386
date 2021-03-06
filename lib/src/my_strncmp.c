/*
** EPITECH PROJECT, 2018
** my_strncmp.c
** File description:
** my_strncmp.c
*/

#include "lib.h"

bool my_strncmp(char const *s1, char const *s2, size_t size)
{
    if (s1 == NULL || s2 == NULL)
        return (false);
    for (; *s1 && *s2 && *s1 == *s2 && size > 0x0; s1++, s2++, size--);
    if ((*s1 == 0x0 && *s2 == 0x0) || size == 0x0)
        return (true);
    return (false);
}
