/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_ldi.c
*/

#include "corewar.h"

static int get_ldi_values(process_t *process, byte_t *memory, int *value1,
    int *value2)
{
    int arg1_type = process->current_op_arg_types[0];
    int arg2_type = process->current_op_arg_types[1];
    int arg1_value = process->current_op_args[0];
    int arg2_value = process->current_op_args[1];

    if ((arg1_type != T_REG && arg1_type != T_DIR && arg1_type != T_IND) ||
        (arg2_type != T_REG && arg2_type != T_DIR))
        return 0;
    *value1 = get_param_value((get_param_value_args_t)
        {process, arg1_type, arg1_value, memory, true});
    *value2 = get_param_value((get_param_value_args_t)
        {process, arg2_type, arg2_value, memory, true});
    return 1;
}

void op_ldi(process_t *process, byte_t *memory, champion_t **champions
    UNUSED, int champion_count UNUSED)
{
    int arg3_type = process->current_op_arg_types[2];
    int arg3_value = process->current_op_args[2];
    int value1;
    int value2;
    int address;
    int result;

    if (arg3_type != T_REG || arg3_value < 1 || arg3_value > REG_NUMBER)
        return;
    if (!get_ldi_values(process, memory, &value1, &value2))
        return;
    address = calculate_address(process->pc, (value1 + value2), true);
    result = read_memory(memory, address, REG_SIZE);
    set_register_value(process, arg3_value, result);
    process->carry = (result == 0);
}
