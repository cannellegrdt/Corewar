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

bool should_dump_memory(vm_t *vm)
{
    return (vm->options.dump_flag &&
        vm->cycle_counter > 0 &&
        vm->cycle_counter % vm->options.dump_cycle == 0);
}

void execute_all_processes(vm_t *vm)
{
    process_t *process = vm->processes;

    while (process) {
        execute_instruction((process->alive) ? vm : NULL, process);
        process = process->next;
    }
}

void run_vm_cycle(vm_t *vm)
{
    int next_check = vm->cycle_to_die;

    if (vm->options.json_output)
        print_json_state(vm);
    while (vm->process_count > 0) {
        if (should_dump_memory(vm))
            dump_memory(vm->memory);
        execute_all_processes(vm);
        vm->cycle_counter++;
        if (vm->options.json_output)
            print_json_state(vm);
        if (vm->cycle_counter < next_check)
            continue;
        check_lives(vm);
        if (vm->cycle_to_die <= 0 || vm->process_count == 0)
            break;
        next_check = vm->cycle_counter + vm->cycle_to_die;
    }
}

void run_vm(vm_t *vm)
{
    run_vm_cycle(vm);
    if (vm->last_alive_champion > 0 && !vm->options.json_output)
        write_win_message(vm);
}