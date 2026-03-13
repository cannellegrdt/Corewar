/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** decode.c
*/

#include "corewar.h"

static bool decode_param(decode_context_t *ctx, int param_index)
{
    int type = get_param_type(ctx, param_index);

    ctx->inst->arg_type[param_index] = type;
    if (type == 0)
        return false;
    if (ctx->has_coding_byte &&
    !(type & op_tab[ctx->opcode - 1].type[param_index]))
        return false;
    return read_param_value(ctx, type, &ctx->inst->args[param_index]);
}

static bool decode_params(decode_context_t *ctx)
{
    int nb_params = op_tab[ctx->opcode - 1].nbr_args;

    for (int j = 0; j < nb_params; j++) {
        if (!decode_param(ctx, j))
            return false;
    }
    return true;
}

int decode_instruction(vm_t *vm, process_t *process,
    instruction_t *instruction)
{
    decode_context_t ctx;

    init_decode_context(&ctx, vm, process, instruction);
    if (!read_coding_byte(&ctx))
        return 0;
    if (!decode_params(&ctx))
        return 0;
    instruction->size = ctx.offset;
    return ctx.offset;
}
