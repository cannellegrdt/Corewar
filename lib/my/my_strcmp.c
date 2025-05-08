/*
** EPITECH PROJECT, 2024
** MY_STRCMP
** File description:
** Write the my_strcmp function.
*/

#include "corewar.h"

int my_strcmp(char const *s1, char const *s2)
{
    if (my_strlen(s1) != my_strlen(s2))
        return 1;
    while (*s1 && *s2) {
        if (*s1 > *s2)
            return 1;
        if (*s1 < *s2)
            return -1;
        s1++;
        s2++;
    }
    return 0;
}
