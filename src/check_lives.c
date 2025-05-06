/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** check_lives.c
*/

#include "corewar.h"

void check_lives(vm_t *vm)
{
    process_t *current = vm->processes;
    process_t *prev = NULL;
    process_t *next;
    bool champion_alive = false;

    while (current) {
        next = current->next;
        champion_alive = false;
        for (int i = 0; i < vm->champion_count; i++) {
            if (vm->champions[i]->number == current->champion_number &&
                vm->champions[i]->alive) {
                champion_alive = true;
                break;
            }
        }
        if (!champion_alive ||
            (current->alive && (vm->cycle_counter - current->last_live_cycle) >
            vm->cycle_to_die)) {
            if (prev)
                prev->next = next;
            else
                vm->processes = next;
            free(current);
            vm->process_count--;
        } else
            prev = current;
        current = next;
    }
    vm->checks_performed++;
    if (vm->lives_in_period >= NBR_LIVE) {
        vm->cycle_to_die -= CYCLE_DELTA;
        vm->lives_in_period = 0;
        vm->checks_performed = 0;
    }
    for (int i = 0; i < vm->champion_count; i++)
        vm->champions[i]->lives_in_period = 0;
}
