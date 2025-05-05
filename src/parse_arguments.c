/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** parse_arguments.c
*/

#include "corewar.h"
#include <stddef.h>
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

static champion_t **allocate_champions_array(int capacity)
{
    champion_t **champs = malloc(sizeof(champion_t *) * capacity);

    if (!champs)
        return NULL;
    return champs;
}

static int cleanup_and_return_error(champion_t *current_champ,
    champion_t **champs, int champ_count, char *error_msg_text)
{
    if (current_champ)
        free(current_champ);
    free_champions(champs, champ_count);
    return error_msg(error_msg_text, 84);
}

static int assign_champion_number(champion_t *champ, int *next_number,
    int champ_count, champion_t **champs)
{
    if (champ->number == -1) {
        *next_number = find_available_number(*next_number, champ_count,
        champs);
        champ->number = *next_number;
        (*next_number)++;
    }
    return 0;
}

static int process_single_champion(int *i, int argc, char **argv,
    champion_t **champs, int *champ_count, int *next_number)
{
    champion_t *champ = init_champion();

    if (!champ)
        return cleanup_and_return_error(NULL, champs, *champ_count,
        "Error: memory allocation failed.\n");
    if (parse_champion_flags(i, argc, argv, champ, *champ_count, champs) != 0)
        return cleanup_and_return_error(champ, champs, *champ_count, NULL);
    if (*i >= argc)
        return cleanup_and_return_error(champ, champs, *champ_count,
        "Error: champion filename expected.\n");
    champ->filename = my_strdup(argv[*i]);
    (*i)++;
    assign_champion_number(champ, next_number, *champ_count, champs);
    champs[*champ_count] = champ;
    (*champ_count)++;
    return 0;
}

int parse_champions(int i, int argc, char **argv, vm_t *vm)
{
    int capacity = (argc - i) / 2 + 1;
    champion_t **champs = allocate_champions_array(capacity);
    int champ_count = 0;
    int next_number = 1;
    int result;

    if (!champs)
        return error_msg("Error: memory allocation failed.\n", 84);
    while (i < argc) {
        result = process_single_champion(&i, argc, argv, champs,
        &champ_count, &next_number);
        if (result != 0)
            return result;
    }
    if (champ_count == 0) {
        free(champs);
        return error_msg("Error: no champions provided.\n", 84);
    }
    vm->champions = champs;
    vm->champion_count = champ_count;
    return 0;
}

int parse_arguments(int argc, char *argv[])
{
    int i = 1;
    vm_t vm;

    initialize_vm(&vm);
    if (parse_dump_flag(&i, argc, argv, &vm) != 0)
        return 84;
    if (parse_champions(i, argc, argv, &vm) != 0)
        return 84;
    if (load_champions(&vm)) {
        free_vm(&vm);
        return 84;
    }
    free_vm(&vm);
    return 0;
}
