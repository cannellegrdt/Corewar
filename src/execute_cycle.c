/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** execute_cycle.c
*/

#include "corewar.h"

void update_wait_cycles_and_load_instructions(vm_t *vm)
{
    process_t *current = vm->processes;

    while (current) {
        if (current->alive) {
            if (current->wait_cycles > 0) {
                current->wait_cycles--;
            } else if (current->current_op == -1) {
                byte_t opcode = vm->memory[current->pc % MEM_SIZE];
                if (opcode >= 1 && opcode <= 16) {
                    current->current_op = opcode;
                    current->wait_cycles = op_tab[opcode - 1].nbr_cycles - 1;
                } else
                    current->pc = (current->pc + 1) % MEM_SIZE;
            }
        }
        current = current->next;
    }
}

static int count_ready_processes(vm_t *vm)
{
    int execution_count = 0;
    process_t *current = vm->processes;

    while (current) {
        if (current->alive && current->wait_cycles == 0 && current->current_op > 0)
            execution_count++;
        current = current->next;
    }
    return execution_count;
}

static void fill_execution_array(vm_t *vm, process_t **processes_array, int array_size)
{
    int idx = 0;
    process_t *current = vm->processes;

    while (current && idx < array_size) {
        if (current->alive && current->wait_cycles == 0 && current->current_op > 0) {
            processes_array[idx] = current;
            idx++;
        }
        current = current->next;
    }
}

void sort_processes_by_champion(process_t **processes_array, int size)
{
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (processes_array[j]->champion_number > processes_array[j + 1]->champion_number) {
                process_t *temp = processes_array[j];
                processes_array[j] = processes_array[j + 1];
                processes_array[j + 1] = temp;
            }
        }
    }
}

static void execute_sorted_instructions(vm_t *vm, process_t **processes_array, int size)
{
    for (int i = 0; i < size; i++) {
        if (processes_array[i]->alive) {
            execute_instruction(vm, processes_array[i]);
            processes_array[i]->current_op = -1;
        }
    }
}

static void execute_unsorted_instructions(vm_t *vm)
{
    process_t *current = vm->processes;

    while (current) {
        if (current->alive && current->wait_cycles == 0 &&
            current->current_op > 0) {
            execute_instruction(vm, current);
            current->current_op = -1;
        }
        current = current->next;
    }
}

void execute_cycle(vm_t *vm)
{
    process_t **processes_to_execute;
    int execution_count;

    update_wait_cycles_and_load_instructions(vm);
    execution_count = count_ready_processes(vm);
    if (execution_count <= 0)
        return;
    processes_to_execute = malloc(sizeof(process_t *) * execution_count);
    if (!processes_to_execute) {
        execute_unsorted_instructions(vm);
        return;
    }
    fill_execution_array(vm, processes_to_execute, execution_count);
    sort_processes_by_champion(processes_to_execute, execution_count);
    execute_sorted_instructions(vm, processes_to_execute, execution_count);
    free(processes_to_execute);
}
