/*
** EPITECH PROJECT, 2017
** my_strcpy
** File description:
** yes
*/

char *my_strcpy(char *dest, char const *src)
{
    for (; *src; src++, dest++)
        *dest = *src;
    *dest = 0;
    return (dest);
}
