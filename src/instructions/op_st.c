/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_st.c
*/

#include "corewar.h"

void op_st(process_t *process, byte_t *memory, champion_t **champions
    __attribute__((unused)), int champion_count __attribute__((unused)))
{
    int arg1_type = process->current_op_arg_types[0];
    int arg2_type = process->current_op_arg_types[1];
    int arg1_value = process->current_op_args[0];
    int arg2_value = process->current_op_args[1];
    int value;
    int address;

    if (arg1_type != T_REG || arg1_value < 1 || arg1_value > REG_NUMBER)
        return;
    value = get_register_value(process, arg1_value);
    if (arg2_type == T_REG) {
        if (arg2_value < 1 || arg2_value > REG_NUMBER)
            return;
        set_register_value(process, arg2_value, value);
        return;
    }
    if (arg2_type == T_IND) {
        address = calculate_address(process->pc, arg2_value, true);
        write_memory(memory, address, value, REG_SIZE);
    }
}
