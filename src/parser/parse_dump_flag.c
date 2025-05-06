/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_dump_flag.c
*/

#include "corewar.h"

int parse_dump_flag(int *i, int argc, char **argv, vm_t *vm)
{
    if (*i < argc && !my_strcmp(argv[*i], "-dump")) {
        if (*i + 1 >= argc)
            return error_msg("Error: -dump requires a cycle number.\n", 84);
        vm->options.dump_flag = true;
        vm->options.dump_cycle = my_getnbr(argv[*i + 1]);
        if (vm->options.dump_cycle <= 0)
            return error_msg("Error: dump cycle must be positive.\n", 84);
        *i += 2;
    }
    return 0;
}
