/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_a_flag.c
*/

#include "corewar.h"

int parse_a_flag(int *i, int argc, char **argv, champion_t *champ)
{
    if (*i + 1 >= argc)
        return error_msg("Error: -a requires an address.\n", 84);
    champ->load_address = my_getnbr(argv[*i + 1]) % MEM_SIZE;
    if (champ->load_address < 0)
        champ->load_address += MEM_SIZE;
    *i += 2;
    return 0;
}
