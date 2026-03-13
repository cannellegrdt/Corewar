/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_champion_flags.c
*/

#include "corewar.h"

int parse_champion_flags(parse_champion_flags_args_t args, vm_t *vm)
{
    parse_n_flag_args_t n_args = {args.i, args.argc, args.argv,
        args.champ, args.champ_count, args.champs};

    while (*args.i < args.argc && (args.argv[*args.i][0] == '-')) {
        if (!my_strcmp(args.argv[*args.i], "--json")) {
            (*args.i)++;
            continue;
        }
        if (!my_strcmp(args.argv[*args.i], "-n") && parse_n_flag(&n_args) != 0)
            return 84;
        if (!my_strcmp(args.argv[*args.i], "-a") && parse_a_flag(&n_args) != 0)
            return 84;
        if (!my_strcmp(args.argv[*args.i], "-dump") &&
            parse_dump_flag(&n_args, vm) != 0)
            return 84;
        if (my_strcmp(args.argv[*args.i], "-n") &&
            my_strcmp(args.argv[*args.i], "-a") &&
            my_strcmp(args.argv[*args.i], "-dump") &&
            my_strcmp(args.argv[*args.i], "--json"))
            break;
        if (*args.i >= args.argc)
            return error_msg(
            "Error: champion filename expected after options.\n", 84);
    }
    return 0;
}