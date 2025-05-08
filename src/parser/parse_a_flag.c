/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_a_flag.c
*/

#include "corewar.h"

int parse_a_flag(int *i, int argc, char **argv, champion_t *champ)
{
    int address;
    char *arg;

    if (*i + 1 >= argc)
        return error_msg("Error: -a requires an address.\n", 84);
    arg = argv[*i + 1];
    if (verify_validity_arg(arg) == 84)
        return 84;
    address = my_getnbr(arg);
    if (address < 0 || address >= MEM_SIZE)
        return error_msg("Error: address must be between 0 and MEM_SIZE-1.\n",
            84);
    champ->load_address = address;
    *i += 2;
    return 0;
}
