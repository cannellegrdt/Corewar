/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** load_champion.c
*/

#include "corewar.h"
#include <stdlib.h>
#include <stdio.h>

void calculate_load_addresses(vm_t *vm)
{
    int spacing = MEM_SIZE / vm->champion_count;

    for (int i = 0; i < vm->champion_count; i++) {
        if (vm->champions[i]->load_address == -1)
            vm->champions[i]->load_address = (i * spacing) % MEM_SIZE;
    }
}

static void free_loaded_champions_code(vm_t *vm, int up_to_index)
{
    for (int j = 0; j <= up_to_index; j++) {
        if (vm->champions[j] && vm->champions[j]->code) {
            free(vm->champions[j]->code);
            vm->champions[j]->code = NULL;
        }
    }
}

static int load_single_champion(vm_t *vm, champion_t *champ, int index)
{
    int address;

    if (!champ) {
        free_loaded_champions_code(vm, index - 1);
        return error_msg("Error: invalid champion.\n", 84);
    }
    if (read_champion_file(champ) != 0) {
        free_loaded_champions_code(vm, index - 1);
        return 84;
    }
    if (!(champ->size <= MEM_SIZE / 6)) {
        free_loaded_champions_code(vm, index);
        return error_msg("Error: champion code is too large.\n", 84);
    }
    for (int j = 0; j < champ->size; j++) {
        address = calculate_address(champ->load_address, j, false);
        vm->memory[address] = champ->code[j];
    }
    create_process(vm, champ);
    return 0;
}

int load_champions(vm_t *vm)
{
    int result;

    if (!(vm && vm->champions && vm->champion_count > 0))
        return error_msg("Error: invalid virtual machine state.\n", 84);
    calculate_load_addresses(vm);
    for (int i = 0; i < vm->champion_count; i++) {
        result = load_single_champion(vm, vm->champions[i], i);
        if (result != 0)
            return result;
    }
    sort_processes_by_champion_number(vm);
    return 0;
}

bool is_number_used(int number, int champ_count,
    champion_t **champs)
{
    for (int j = 0; j < champ_count; j++) {
        if (champs[j]->number == number)
            return true;
    }
    return false;
}

int find_available_number(int next_number, int champ_count,
    champion_t **champs)
{
    while (is_number_used(next_number, champ_count, champs))
        next_number++;
    return next_number;
}
