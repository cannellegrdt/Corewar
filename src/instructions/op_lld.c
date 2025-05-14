/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_lld.c
*/

#include "corewar.h"

void op_lld(process_t *process, byte_t *memory, champion_t **champions UNUSED,
    int champion_count UNUSED)
{
    int arg1_type = process->current_op_arg_types[0];
    int arg2_type = process->current_op_arg_types[1];
    int arg1_value = process->current_op_args[0];
    int arg2_value = process->current_op_args[1];
    int result;
    int address;

    if (arg2_type != T_REG || arg2_value < 1 || arg2_value > REG_NUMBER)
        return;
    if (arg1_type == T_DIR)
        result = arg1_value;
    else if (arg1_type == T_IND) {
        address = calculate_address(process->pc, arg1_value, false);
        result = read_memory(memory, address, REG_SIZE);
    } else
        return;
    set_register_value(process, arg2_value, result);
    process->carry = (result == 0);
}
