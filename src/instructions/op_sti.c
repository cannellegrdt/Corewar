/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_sti.c
*/

#include "corewar.h"

void op_sti(process_t *process, byte_t *memory, champion_t **champions UNUSED,
    int champion_count UNUSED)
{
    int arg1_type = process->current_op_arg_types[0];
    int arg2_type = process->current_op_arg_types[1];
    int arg3_type = process->current_op_arg_types[2];
    int arg1_value = process->current_op_args[0];
    int arg2_value = process->current_op_args[1];
    int arg3_value = process->current_op_args[2];
    int register_value;
    int value2;
    int value3;
    int address;

    if (arg1_type != T_REG || arg1_value < 1 || arg1_value > REG_NUMBER)
        return;
    register_value = get_register_value(process, arg1_value);
    value2 = get_param_value((get_param_value_args_t)
        {process, arg2_type, arg2_value, memory, true});
    value3 = get_param_value((get_param_value_args_t)
        {process, arg3_type, arg3_value, memory, true});
    address = calculate_address(process->pc, (value2 + value3), true);
    write_memory(memory, address, register_value, REG_SIZE);
}
