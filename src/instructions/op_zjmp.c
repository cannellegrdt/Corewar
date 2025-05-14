/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_zjmp.c
*/

#include "corewar.h"

void op_zjmp(process_t *process, byte_t *memory UNUSED,
    champion_t **champions UNUSED, int champion_count UNUSED)
{
    int arg_value = process->current_op_args[0];
    int new_pc;

    if (process->carry) {
        new_pc = calculate_address(process->pc, arg_value, true);
        process->pc = new_pc;
    }
}
