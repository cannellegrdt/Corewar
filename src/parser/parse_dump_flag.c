/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_dump_flag.c
*/

#include "corewar.h"

int verify_validity_arg(char *arg)
{
    int j = 0;

    if (arg[j] == '+')
        j++;
    if (arg[j] == '-' || arg[j] == '\0')
        return error_msg("Error: invalid argument.\n", 84);
    while (arg[j]) {
        if (arg[j] < '0' || arg[j] > '9')
            return error_msg("Error: invalid argument.\n", 84);
        j++;
    }
    return 0;
}

int parse_dump_flag(parse_n_flag_args_t *args, vm_t *vm)
{
    char *arg;
    int cycle;

    if (*args->i + 1 >= args->argc)
        return error_msg("Error: -dump requires a cycle number.\n", 84);
    arg = args->argv[*args->i + 1];
    if (verify_validity_arg(arg) == 84)
        return 84;
    cycle = my_getnbr(arg);
    if (cycle <= 0)
        return error_msg("Error: dump cycle must be positive.\n", 84);
    vm->options.dump_flag = true;
    vm->options.dump_cycle = cycle;
    *args->i += 2;
    return 0;
}
