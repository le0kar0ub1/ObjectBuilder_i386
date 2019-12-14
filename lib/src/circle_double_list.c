/*
** EPITECH PROJECT, 2018
** pushswap.c
** File description:
** swap
*/

#include "lib.h"

list *circle_double_list(char **argv, int a)
{
    list *element;
    list *ret = malloc(sizeof(list));
    ret->data = my_getnbr(argv[0]);
    ret->next = ret;

    for (int i = a - 1; i > 0; i--) {
        element = malloc(sizeof(list));
        element->data = my_getnbr(argv[i]);
        element->next = ret->next;
        ret->next = element;
        (element->next)->prev = element;
    }
    element->prev = ret;
    return (ret);
}
