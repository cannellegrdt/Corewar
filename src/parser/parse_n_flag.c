/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_n_flag.c
*/

#include "corewar.h"

int parse_n_flag(parse_n_flag_args_t *args)
{
    char *arg;
    int number;

    if (*args->i + 1 >= args->argc)
        return error_msg("Error: -n requires a number.\n", 84);
    arg = args->argv[*args->i + 1];
    if (verify_validity_arg(arg) == 84)
        return 84;
    number = my_getnbr(arg);
    if (number <= 0)
        return error_msg("Error: champion number must be positive.\n", 84);
    for (int j = 0; j < args->champ_count; j++) {
        if (args->champs[j]->number == number)
            return error_msg("Error: duplicate champion number.\n", 84);
    }
    args->champ->number = number;
    *args->i += 2;
    return 0;
}
