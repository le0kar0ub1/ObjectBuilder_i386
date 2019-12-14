/*
** EPITECH PROJECT, 2018
** params_to_circle_list.c
** File description:
** params_to_circle_list
*/

#include "lib.h"

list *my_params_to_list(char **argv, int argc)
{
    int a = argc - 1;
    list *element;
    list *list = malloc(sizeof(list));
    list = NULL;

    while (a > 0) {
        element = malloc(sizeof(list));
        element->data =  my_getnbr(argv[a]);
        element->next = (list != NULL ? list : NULL);
        list = element;
        a = a - 1;
    }
    while (element->next != NULL)
        element = element->next;
    element->next = list;
    return (list);
}
