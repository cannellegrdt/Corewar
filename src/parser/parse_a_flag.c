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
    int j = 0;

    if (*i + 1 >= argc)
        return error_msg("Error: -a requires an address.\n", 84);
    arg = argv[*i + 1];
    j += (arg[j] == '-' || arg[j] == '+') ? 1 : 0;
    while (arg[j]) {
        if (arg[j] < '0' || arg[j] > '9')
            return error_msg("Error: not a number.\n", 84);
        j++;
    }
    address = my_getnbr(arg);
    if (address < 0 || address >= MEM_SIZE)
        return error_msg("Error: address must be between 0 and MEM_SIZE-1.\n",
            84);
    champ->load_address = address;
    *i += 2;
    return 0;
}
