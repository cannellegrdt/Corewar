/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** check_lives.c
*/

#include "corewar.h"

static bool should_kill_process(vm_t *vm, process_t *process)
{
    bool champion_alive = false;

    for (int i = 0; i < vm->champion_count; i++) {
        if (vm->champions[i]->number == process->champion_number &&
            vm->champions[i]->alive) {
            champion_alive = true;
            break;
        }
    }
    return (!champion_alive ||
        (process->alive && (vm->cycle_counter - process->last_live_cycle) >
        vm->cycle_to_die));
}

static void clean_processes(vm_t *vm)
{
    process_t *current = vm->processes;
    process_t *prev = NULL;
    process_t *next;

    while (current) {
        next = current->next;
        if (!should_kill_process(vm, current)) {
            prev = current;
            current = next;
            continue;
        }
        if (prev)
            prev->next = next;
        else
            vm->processes = next;
        free(current);
        vm->process_count--;
        current = next;
    }
}

void check_lives(vm_t *vm)
{
    clean_processes(vm);
    vm->checks_performed++;
    if (vm->lives_in_period >= NBR_LIVE) {
        vm->cycle_to_die -= CYCLE_DELTA;
        vm->lives_in_period = 0;
        vm->checks_performed = 0;
    }
    for (int i = 0; i < vm->champion_count; i++)
        vm->champions[i]->lives_in_period = 0;
}
