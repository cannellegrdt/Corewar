/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** decode_args.c
*/

#include "corewar.h"

static void initialize_arg_arrays(process_t *process)
{
    for (int i = 0; i < MAX_ARGS_NUMBER; i++) {
        process->current_op_arg_types[i] = 0;
        process->current_op_args[i] = 0;
    }
}

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
    if (!(process->current_op_args[i] >= 1 &&
    process->current_op_args[i] <= REG_NUMBER))
        return offset + 1;
    return offset + 1;
}

static int read_direct_arg(vm_t *vm, process_t *process, int i, int offset,
    byte_t opcode)
{
    int dir_size = (opcode == 9 || opcode == 10 || opcode == 11 ||
    opcode == 14) ? 2 : 4;

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

static int decode_args_with_coding_byte(vm_t *vm, process_t *process,
    const op_t *op, int offset)
{
    byte_t coding_byte = vm->memory[(process->pc + offset) % MEM_SIZE];
    int start_offset = offset;

    offset++;
    for (int i = 0; i < op->nbr_args; i++) {
        int arg_type = convert_arg_code_to_type((coding_byte >> (6 - 2 * i))
        & 0x3);
        if (arg_type == 0 || !((arg_type & op->type[i]) != 0))
            return start_offset + 1;
        process->current_op_arg_types[i] = arg_type;
        if (arg_type == T_REG) {
            offset = read_register_arg(vm, process, i, offset);
            if (!(process->current_op_args[i] >= 1 &&
            process->current_op_args[i] <= REG_NUMBER))
                return offset;
        } else if (arg_type == T_DIR)
            offset = read_direct_arg(vm, process, i, offset, vm->memory[process->pc]);
        else if (arg_type == T_IND)
            offset = read_indirect_arg(vm, process, i, offset);
    }
    return offset;
}

static int decode_args_without_coding_byte(vm_t *vm, process_t *process,
    byte_t opcode, int offset)
{
    process->current_op_arg_types[0] = T_DIR;
    if (opcode == 9 || opcode == 12 || opcode == 15) {
        process->current_op_args[0] = read_short(vm->memory,
        process->pc + offset);
        offset += 2;
    } else {
        process->current_op_args[0] = read_int(vm->memory,
        process->pc + offset);
        offset += 4;
    }
    return offset;
}

int decode_args(vm_t *vm, process_t *process)
{
    byte_t opcode = vm->memory[process->pc];
    const op_t *op;
    int offset = 1;

    if (!(opcode >= 1 && opcode <= 16))
        return 1;
    op = &op_tab[opcode - 1];
    offset = 1;
    initialize_arg_arrays(process);
    if (opcode != 1 && opcode != 9 && opcode != 12 && opcode != 15)
        offset = decode_args_with_coding_byte(vm, process, op, offset);
    else
        offset = decode_args_without_coding_byte(vm, process, opcode, offset);
    return offset;
}
