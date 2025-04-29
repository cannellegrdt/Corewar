/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** error_msg.c
*/

#include "corewar.h"
#include <unistd.h>

int error_msg(char *str, int ret_value)
{
    if (str)
        write(2, str, my_strlen(str));
    return ret_value;
}
