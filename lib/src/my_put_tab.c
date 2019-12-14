/*
** EPITECH PROJECT, 2018
** bsq_prime.c
** File description:
** bsq_prime
*/

#include "lib.h"

void my_put_tab(char **tab)
{
    for (; *tab; tab++) {
        my_putstr(*tab);
        my_putchar('\n');
    }
}
