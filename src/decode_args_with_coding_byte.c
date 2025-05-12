/*
** EPITECH PROJECT, 2025
** Corewar
** File description:
** decode_args_with_coding_byte.c
*/

#include "corewar.h"

static int convert_arg_code_to_type(int arg_code)
{
    switch (arg_code) {
        case 1:
            return T_REG;
        case 2:
            return T_DIR;
        case 3:
            return T_IND;
        default:
            return 0;
    }
}

static int read_register_arg(vm_t *vm, process_t *process, int i, int offset)
{
    process->current_op_args[i] = vm->memory[(process->pc + offset) %
        MEM_SIZE];
    return offset + 1;
}

static int read_direct_arg(vm_t *vm, process_t *process, int i, int offset)
{
    int dir_size = (vm->memory[process->pc] == 9 ||
        vm->memory[process->pc] == 10 ||
        vm->memory[process->pc] == 11 ||
        vm->memory[process->pc] == 14) ? 2 : 4;

    if (dir_size == 2)
        process->current_op_args[i] = read_short(vm->memory,
        process->pc + offset);
    else
        process->current_op_args[i] = read_int(vm->memory,
        process->pc + offset);
    return offset + dir_size;
}

static int read_indirect_arg(vm_t *vm, process_t *process, int i, int offset)
{
    process->current_op_args[i] = read_short(vm->memory, process->pc + offset);
    return offset + IND_SIZE;
}

static bool is_valid_register(int reg_value)
{
    return reg_value >= 1 && reg_value <= REG_NUMBER;
}

static int process_single_argument(vm_t *vm, process_t *process,
    int arg_index, int arg_type, int offset)
{
    process->current_op_arg_types[arg_index] = arg_type;
    if (arg_type == T_REG) {
        offset = read_register_arg(vm, process, arg_index, offset);
        if (!is_valid_register(process->current_op_args[arg_index]))
            return offset;
    } else if (arg_type == T_DIR)
        return read_direct_arg(vm, process, arg_index, offset);
    if (arg_type == T_IND)
        return read_indirect_arg(vm, process, arg_index, offset);
    return offset;
}

int decode_args_with_coding_byte(vm_t *vm, process_t *process, const op_t *op,
    int offset)
{
    byte_t coding_byte = vm->memory[(process->pc + offset) % MEM_SIZE];
    int start_offset = offset;
    int arg_type;

    offset++;
    for (int i = 0; i < op->nbr_args; i++) {
        arg_type = convert_arg_code_to_type((coding_byte >> (6 - 2 * i)) &
        0x3);
        if (!(arg_type != 0 && ((arg_type & op->type[i]) != 0)))
            return start_offset + 1;
        offset = process_single_argument(vm, process, i, arg_type, offset);
        if (arg_type == T_REG &&
            !is_valid_register(process->current_op_args[i]))
            return offset;
    }
    return offset;
}
