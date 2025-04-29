/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** init_and_free.c
*/

#include "corewar.h"
#include <stdlib.h>

void free_champions(champion_t **champions, int count)
{
    if (!champions)
        return;
    for (int i = 0; i < count; i++) {
        if (champions[i]) {
            free((champions[i]->code) ? champions[i]->code : NULL);
            free((champions[i]->filename) ? champions[i]->filename : NULL);
            free(champions[i]);
        }
    }
    free(champions);
}

void free_vm(vm_t *vm)
{
    process_t *next;
    process_t *current;

    if (!vm)
        return;
    free_champions(vm->champions, vm->champion_count);
    current = vm->processes;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

void initialize_vm(vm_t *vm)
{
    my_memset(vm->memory, 0, MEM_SIZE);
    vm->champions = NULL;
    vm->champion_count = 0;
    vm->processes = NULL;
    vm->process_count = 0;
    vm->cycle_counter = 0;
    vm->cycle_to_die = CYCLE_TO_DIE;
    vm->checks_performed = 0;
    vm->lives_in_period = 0;
    vm->last_alive_champion = 0;
    vm->options.dump_flag = false;
    vm->options.dump_cycle = 0;
    //initialize_op(vm);
}
