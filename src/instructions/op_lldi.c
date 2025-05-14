/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_lldi.c
*/

#include "corewar.h"

static bool validate_lldi_args(process_t *process)
{
    int arg1_type = process->current_op_arg_types[0];
    int arg2_type = process->current_op_arg_types[1];
    int arg3_type = process->current_op_arg_types[2];
    int arg3_value = process->current_op_args[2];

    return !(arg3_type != T_REG || arg3_value < 1 || arg3_value > REG_NUMBER ||
            (arg1_type != T_REG && arg1_type != T_DIR && arg1_type != T_IND) ||
            (arg2_type != T_REG && arg2_type != T_DIR));
}

void op_lldi(process_t *process, byte_t *memory, champion_t **champions
    UNUSED, int champion_count UNUSED)
{
    int arg1_type = process->current_op_arg_types[0];
    int arg2_type = process->current_op_arg_types[1];
    int arg1_value = process->current_op_args[0];
    int arg2_value = process->current_op_args[1];
    int arg3_value = process->current_op_args[2];
    int value1 = get_param_value((get_param_value_args_t)
        {process, arg1_type, arg1_value, memory, true});
    int value2 = get_param_value((get_param_value_args_t)
        {process, arg2_type, arg2_value, memory, true});
    int address = calculate_address(process->pc, (value1 + value2), false);
    int result = read_memory(memory, address, REG_SIZE);

    if (!validate_lldi_args(process))
        return;
    set_register_value(process, arg3_value, result);
    process->carry = (result == 0);
}
