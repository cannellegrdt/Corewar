/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-robotfactory-thomas.sauge-merle
** File description:
** endswith.c
*/

#include "corewar.h"

int endswith(char *str, char *find)
{
    int len_str = my_strlen(str);
    int len_find = my_strlen(find);

    if (len_str < len_find)
        return 0;
    if (len_str == len_find)
        return (my_strcmp(str, find) == 0) ? 1 : 0;
    for (int i = 0; i < len_find; i++) {
        if (str[len_str - len_find + i] != find[i])
            return 0;
    }
    return 1;
}
