/*
** EPITECH PROJECT, 2017
** my_strstr
** File description:
** yes
*/

#include "lib.h"

char *my_strstr(char *str, char *find)
{
    int k = 0;

    while (*str && find[k]) {
        if (*str == find[k])
            k++;
        else
            k = 0;
        str++;
    }
    if (find[k] == 0)
        return (str - k);
    return (NULL);
}
