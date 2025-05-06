/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_champion_flags.c
*/

#include "corewar.h"

int parse_champion_flags(parse_champion_flags_args_t args)
{
    parse_n_flag_args_t n_args = {args.i, args.argc, args.argv,
        args.champ, args.champ_count, args.champs};

    while (*args.i < args.argc && (args.argv[*args.i][0] == '-')) {
        if (!my_strcmp(args.argv[*args.i], "-n") &&
            parse_n_flag(&n_args) != 0)
            return 84;
        if (!my_strcmp(args.argv[*args.i], "-a") &&
            parse_a_flag(args.i, args.argc, args.argv, args.champ) != 0)
            return 84;
        if (!my_strcmp(args.argv[*args.i], "-n") &&
            !my_strcmp(args.argv[*args.i], "-a"))
            break;
    }
    return 0;
}
