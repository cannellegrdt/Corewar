/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** execute_cycle.c
*/

#include "corewar.h"

static void update_process_wait_cycle(process_t *process, byte_t opcode)
{
    if (opcode >= 1 && opcode <= 16) {
        process->current_op = opcode;
        process->wait_cycles = op_tab[opcode - 1].nbr_cycles - 1;
    } else
        process->pc = (process->pc + 1) % MEM_SIZE;
}

static void handle_process(vm_t *vm, process_t *process)
{
    byte_t opcode;

    if (!process->alive)
        return;
    if (process->wait_cycles > 0) {
        process->wait_cycles--;
    } else if (process->current_op == -1) {
        opcode = vm->memory[process->pc % MEM_SIZE];
        update_process_wait_cycle(process, opcode);
    }
}

void update_wait_cycles_and_load_instructions(vm_t *vm)
{
    process_t *current = vm->processes;

    while (current) {
        handle_process(vm, current);
        current = current->next;
    }
}

static int count_ready_processes(vm_t *vm)
{
    int execution_count = 0;
    process_t *current = vm->processes;

    while (current) {
        if (current->alive && current->wait_cycles == 0 &&
            current->current_op > 0)
            execution_count++;
        current = current->next;
    }
    return execution_count;
}

static void fill_execution_array(vm_t *vm, process_t **processes_array,
    int array_size)
{
    int idx = 0;
    process_t *current = vm->processes;

    while (current && idx < array_size) {
        if (current->alive && current->wait_cycles == 0 &&
            current->current_op > 0) {
            processes_array[idx] = current;
            idx++;
        }
        current = current->next;
    }
}

void sort_processes_by_champion(process_t **processes_array, int size)
{
    bool swapped;
    process_t *temp;

    do {
        swapped = false;
        for (int j = 0; j < size - 1; j++) {
            temp = processes_array[j + 1];
            processes_array[j + 1] =
                (processes_array[j]->champion_number > temp->champion_number) ?
                processes_array[j] : temp;
            processes_array[j] =
                (processes_array[j]->champion_number > temp->champion_number)
                ? temp : processes_array[j];
            swapped = swapped ||
                (processes_array[j]->champion_number > temp->champion_number);
        }
        size--;
    } while (swapped);
}

static void execute_sorted_instructions(vm_t *vm, process_t **processes_array,
    int size)
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
