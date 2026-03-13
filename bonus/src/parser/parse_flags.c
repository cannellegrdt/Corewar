/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** parse_n_flag.c
*/

#include "corewar.h"

static int verify_validity_arg(char *arg)
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

static int parse_dump_flag(parse_n_flag_args_t *args, vm_t *vm)
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

static int parse_a_flag(parse_n_flag_args_t *args)
{
    int address;
    char *arg;

    if (*args->i + 1 >= args->argc)
        return error_msg("Error: -a requires an address.\n", 84);
    arg = args->argv[*args->i + 1];
    if (verify_validity_arg(arg) == 84)
        return 84;
    address = my_getnbr(arg);
    if (address < 0 || address >= MEM_SIZE)
        return error_msg("Error: address must be between 0 and MEM_SIZE-1.\n",
            84);
    args->champ->load_address = address;
    *args->i += 2;
    return 0;
}

static int parse_n_flag(parse_n_flag_args_t *args)
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

int parse_champion_flags(parse_champion_flags_args_t args, vm_t *vm)
{
    parse_n_flag_args_t n_args = {args.i, args.argc, args.argv,
        args.champ, args.champ_count, args.champs};

    while (*args.i < args.argc && (args.argv[*args.i][0] == '-')) {
        if (!my_strcmp(args.argv[*args.i], "--json")) {
            vm->options.json_output = true;
            (*args.i)++;
            return 0;
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
