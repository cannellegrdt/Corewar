/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** parse_arguments.c
*/

#include "corewar.h"
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

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

int parse_n_flag(int *i, int argc, char **argv, champion_t *champ,
    int champ_count, champion_t **champs)
{
    if (*i + 1 >= argc)
        return error_msg("Error: -n requires a number.\n", 84);
    champ->number = my_getnbr(argv[*i + 1]);
    if (champ->number <= 0)
        return error_msg("Error: champion number must be positive.\n", 84);
    for (int j = 0; j < champ_count; j++) {
        if (champs[j]->number == champ->number)
            return error_msg("Error: duplicate champion number.\n", 84);
    }
    *i += 2;
    return 0;
}

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

int parse_champion_flags(int *i, int argc, char **argv, champion_t *champ,
    int champ_count, champion_t **champs)
{
    while (*i < argc && (argv[*i][0] == '-')) {
        if (!my_strcmp(argv[*i], "-n") &&
        parse_n_flag(i, argc, argv, champ, champ_count, champs) != 0)
            return 84;
        if (!my_strcmp(argv[*i], "-a") &&
        parse_a_flag(i, argc, argv, champ) != 0)
            return 84;
        if (!my_strcmp(argv[*i], "-n") && !my_strcmp(argv[*i], "-a"))
            break;
    }
    return 0;
}

int parse_arguments(int argc, char *argv[])
{
    int i = 1;
    vm_t vm;

    initialize_vm(&vm);
    if (parse_dump_flag(&i, argc, argv, &vm) != 0)
        return 84;
    //run_vm(&vm);
    free_vm(&vm);
    return 0;
}
