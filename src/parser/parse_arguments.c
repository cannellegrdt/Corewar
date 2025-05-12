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

static int cleanup_and_return_error(champion_t *current_champ,
    champion_t **champs, int champ_count, char *error_msg_text)
{
    if (current_champ)
        free(current_champ);
    free_champions(champs, champ_count);
    return error_msg(error_msg_text, 84);
}

static int assign_champion_number(champion_t *champ, int *next_num,
    int champ_count, champion_t **champs)
{
    if (champ->number == -1) {
        *next_num = find_available_number(*next_num, champ_count,
        champs);
        champ->number = *next_num;
        (*next_num)++;
    }
    return 0;
}

static int process_single_champion(process_single_champion_args_t args,
    vm_t *vm)
{
    champion_t *champ = init_champion();
    int next_num = 1;

    if (!champ)
        return cleanup_and_return_error(NULL, args.champs, *args.champ_count,
        "Error: memory allocation failed.\n");
    if (parse_champion_flags((parse_champion_flags_args_t){args.i, args.argc,
        args.argv, champ, *args.champ_count, args.champs}, vm) != 0)
        return cleanup_and_return_error(champ, args.champs, *args.champ_count,
            NULL);
    if (*args.i >= args.argc)
        return cleanup_and_return_error(champ, args.champs, *args.champ_count,
        "Error: champion filename expected.\n");
    if (args.argv[*args.i][0] == '-')
        return cleanup_and_return_error(champ, args.champs, *args.champ_count,
        "Error: Unknown flag.\n");
    champ->filename = my_strdup(args.argv[*args.i]);
    (*args.i)++;
    assign_champion_number(champ, &next_num, *args.champ_count, args.champs);
    args.champs[*args.champ_count] = champ;
    (*args.champ_count)++;
    return 0;
}

static int parse_champions(int i, int argc, char **argv, vm_t *vm)
{
    int capacity = argc;
    champion_t **champs = malloc(sizeof(champion_t *) * capacity);
    int champ_count = 0;
    int result;

    if (!champs)
        return error_msg("Error: memory allocation failed.\n", 84);
    while (i < argc) {
        result = process_single_champion((process_single_champion_args_t){
            &i, argc, argv, champs, &champ_count}, vm);
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
    int champion_count = 0;

    initialize_vm(&vm);
    for (int j = 1; j < argc; j++) {
        if (endswith(argv[j], ".cor"))
            champion_count++;
    }
    if (champion_count == 1)
        return error_msg("Error: at least two champions are needed.\n", 84);
    if (parse_champions(i, argc, argv, &vm) != 0)
        return 84;
    if (load_champions(&vm)) {
        free_vm(&vm);
        return 84;
    }
    run_vm(&vm);
    free_vm(&vm);
    return 0;
}
