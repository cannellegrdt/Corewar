/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** decode_args.c
*/

#include "corewar.h"

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
    for (int i = 0; i < MAX_ARGS_NUMBER; i++) {
        process->current_op_arg_types[i] = 0;
        process->current_op_args[i] = 0;
    }
    if (opcode != 1 && opcode != 9 && opcode != 12 && opcode != 15)
        offset = decode_args_with_coding_byte(vm, process, op, offset);
    else
        offset = decode_args_without_coding_byte(vm, process, opcode, offset);
    return offset;
}
