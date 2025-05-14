/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_lfork.c
*/

#include "corewar.h"
#include <stdlib.h>

void op_lfork(process_t *process, byte_t *memory UNUSED,
    champion_t **champions UNUSED, int champion_count UNUSED)
{
    int arg1_value = process->current_op_args[0];
    process_t *new_process = malloc(sizeof(process_t));

    if (!new_process)
        return;
    *new_process = *process;
    new_process->pc = calculate_address(process->pc, arg1_value, false);
    new_process->wait_cycles = 0;
    new_process->current_op = -1;
    for (int i = 0; i < REG_NUMBER; i++)
        new_process->registers[i] = process->registers[i];
    new_process->next = process->vm->processes;
    new_process->last_live_cycle = process->last_live_cycle;
    process->vm->processes = new_process;
    process->vm->process_count++;
}
