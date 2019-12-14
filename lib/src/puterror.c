/*
** EPITECH PROJECT, 2018
** my_printf.c
** File description:
** my_printf
*/

#include "lib.h"

void puterror(char *s)
{
    for (; *s; s++)
        write(2, s, 1);
}
