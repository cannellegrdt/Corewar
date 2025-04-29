/*
** EPITECH PROJECT, 2024
** MY_STRDUP
** File description:
** Write the strdup function.
*/

#include <stdlib.h>

char *my_strdup(char *str)
{
    int len = 0;
    char *dup;

    while (str[len])
        len++;
    dup = (char *)malloc(len + 1);
    if (!dup)
        return NULL;
    for (int i = 0; i <= len; i++)
        dup[i] = str[i];
    return dup;
}
