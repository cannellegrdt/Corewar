/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** run_vm.c
*/

#include "corewar.h"
#include <stdlib.h>
#include <unistd.h>

bool has_alive_process(vm_t *vm)
{
    process_t *current = vm->processes;

    if (vm->cycle_to_die <= 0) {
        for (int i = 0; i < vm->champion_count; i++) {
            if (vm->champions[i]->lives_in_period > 0)
                return true;
        }
        return false;
    }
    while (current) {
        if (current->alive)
            return true;
        current = current->next;
    }
    return false;
}

static void update_wait_cycles_and_load_instructions(vm_t *vm)
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

static void sort_processes_by_champion(process_t **processes_array, int size)
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
            //execute_instruction(vm, processes_array[i]);
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
            //execute_instruction(vm, current);
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

void check_lives(vm_t *vm)
{
    process_t *current = vm->processes;
    process_t *prev = NULL;
    process_t *next;

    while (current) {
        next = current->next;
        if (current->alive && (vm->cycle_counter - current->last_live_cycle) > vm->cycle_to_die) {
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

static void write_win_message(vm_t *vm)
{
    for (int i = 0; i < vm->champion_count; i++) {
        if (vm->champions[i]->number == vm->last_alive_champion) {
            write(1, "The player ", 11);
            my_putnbr(vm->last_alive_champion);
            write(1, "(", 1);
            write(1, vm->champions[i]->name,
            my_strlen(vm->champions[i]->name));
            write(1, ") has won.\n", 12);
            break;
        }
    }
}

void run_vm(vm_t *vm)
{
    int next_check = vm->cycle_to_die;
    process_t *process;

    while (vm->process_count > 0) {
        if (vm->options.dump_flag && vm->cycle_counter == vm->options.dump_cycle) {
            dump_memory(vm->memory);
            break;
        }
        process = vm->processes;
        while (process) {
            //execute_instruction((process->alive) ? vm : NULL, process);
            process = process->next;
        }
        vm->cycle_counter++;
        if (vm->cycle_counter >= next_check) {
            check_lives(vm);
            if (vm->cycle_to_die <= 0 || vm->process_count == 0)
                break;
            next_check = vm->cycle_counter + vm->cycle_to_die;
        }
    }
    if (vm->last_alive_champion > 0)
        write_win_message(vm);
}
