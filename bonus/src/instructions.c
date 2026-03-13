/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** instructions.c
*/

#include "corewar.h"
#include <unistd.h>

int get_param_type(decode_context_t *ctx, int param_index)
{
    byte_t param_type;

    if (!ctx->has_coding_byte) {
        if (ctx->opcode == 1 || ctx->opcode == 9 || ctx->opcode == 12 ||
        ctx->opcode == 15)
            return T_DIR;
        return 0;
    }
    param_type = (ctx->coding_byte >> (6 - 2 * param_index)) & 0x3;
    switch (param_type) {
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

static void initialize_wait_cycles(process_t *process, byte_t opcode)
{
    if (process->wait_cycles == 0) {
        process->current_op = opcode - 1;
        process->wait_cycles = op_tab[opcode - 1].nbr_cycles;
    }
}

static bool decrease_wait_cycles(process_t *process)
{
    process->wait_cycles--;
    return (process->wait_cycles <= 0);
}

static int perform_operation(vm_t *vm, process_t *process, byte_t opcode)
{
    int instruction_size = decode_args(vm, process);

    if (instruction_size > 0)
        vm->op_functions[opcode - 1](process, vm->memory, vm->champions,
        vm->champion_count);
    return instruction_size;
}

static void update_program_counter(process_t *process, byte_t opcode,
    int instruction_size)
{
    if (!process)
        return;
    if (opcode != 9 || !process->carry) {
        process->pc = (process->pc + instruction_size) % MEM_SIZE;
        if (process->pc < 0)
            process->pc += MEM_SIZE;
    }
}

static void skip_invalid_opcode(process_t *process)
{
    process->pc = (process->pc + 1) % MEM_SIZE;
    if (process->pc < 0)
        process->pc += MEM_SIZE;
}

void execute_instruction(vm_t *vm, process_t *process)
{
    byte_t opcode;
    int instruction_size;

    if (!vm || !process)
        return;
    opcode = vm->memory[process->pc];
    if (opcode >= 1 && opcode <= 16) {
        initialize_wait_cycles(process, opcode);
        if (decrease_wait_cycles(process)) {
            instruction_size = perform_operation(vm, process, opcode);
            update_program_counter((instruction_size > 0) ? process : NULL,
            opcode, instruction_size);
            process->current_op = -1;
        }
    } else
        skip_invalid_opcode(process);
}
