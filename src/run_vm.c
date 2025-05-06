/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** run_vm.c
*/

#include "corewar.h"
#include <stdlib.h>
#include <unistd.h>

static void write_win_message(vm_t *vm)
{
    for (int i = 0; i < vm->champion_count; i++) {
        if (vm->champions[i]->number != vm->last_alive_champion)
            continue;
        write(1, "The player ", 11);
        my_putnbr(vm->last_alive_champion);
        write(1, "(", 1);
        write(1, vm->champions[i]->name,
        my_strlen(vm->champions[i]->name));
        write(1, ") has won.\n", 12);
        break;
    }
}

void run_vm(vm_t *vm)
{
    int next_check = vm->cycle_to_die;
    process_t *process;

    while (vm->process_count > 0) {
        if (vm->options.dump_flag &&
            vm->cycle_counter == vm->options.dump_cycle) {
            dump_memory(vm->memory);
            break;
        }
        process = vm->processes;
        while (process) {
            execute_instruction((process->alive) ? vm : NULL, process);
            process = process->next;
        }
        vm->cycle_counter++;
        if (vm->cycle_counter < next_check)
            continue;
        check_lives(vm);
        if (vm->cycle_to_die <= 0 || vm->process_count == 0)
            break;
        next_check = vm->cycle_counter + vm->cycle_to_die;
    }
    if (vm->last_alive_champion > 0)
        write_win_message(vm);
}
