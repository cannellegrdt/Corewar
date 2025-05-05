/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** read_params.c
*/

#include "corewar.h"

bool read_coding_byte(decode_context_t *ctx)
{
    ctx->has_coding_byte = (ctx->opcode != 1 && ctx->opcode != 9 &&
    ctx->opcode != 12 && ctx->opcode != 15);
    if (ctx->has_coding_byte) {
        ctx->coding_byte = ctx->vm->memory[(ctx->pc + ctx->offset) % MEM_SIZE];
        ctx->offset++;
    } else
        ctx->coding_byte = 0;
    return true;
}

static bool read_register_param(decode_context_t *ctx, int *arg)
{
    *arg = ctx->vm->memory[(ctx->pc + ctx->offset) % MEM_SIZE];
    ctx->offset++;
    return (*arg >= 1 && *arg <= REG_NUMBER);
}

static void read_direct_param(decode_context_t *ctx, int *arg)
{
    if (ctx->opcode == 10 || ctx->opcode == 11 || ctx->opcode == 14) {
        *arg = read_short(ctx->vm->memory, (ctx->pc + ctx->offset) % MEM_SIZE);
        ctx->offset += IND_SIZE;
    } else {
        *arg = read_int(ctx->vm->memory, (ctx->pc + ctx->offset) % MEM_SIZE);
        ctx->offset += DIR_SIZE;
    }
}

static void read_indirect_param(decode_context_t *ctx, int *arg)
{
    *arg = read_short(ctx->vm->memory, (ctx->pc + ctx->offset) % MEM_SIZE);
    ctx->offset += IND_SIZE;
}

bool read_param_value(decode_context_t *ctx, int arg_type,
    int *arg_value)
{
    switch (arg_type) {
        case T_REG:
            return read_register_param(ctx, arg_value);
        case T_DIR:
            read_direct_param(ctx, arg_value);
            return true;
        case T_IND:
            read_indirect_param(ctx, arg_value);
            return true;
        default:
            return false;
    }
}
