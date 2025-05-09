/*
** EPITECH PROJECT, 2024
** Corewar
** File description:
** creates.c
*/

#include "corewar.h"
#include <stdlib.h>

void create_process(vm_t *vm, champion_t *champ)
{
    process_t *process = malloc(sizeof(process_t));

    if (!process)
        return;
    process->champion_number = champ->number;
    process->pc = champ->load_address;
    for (int i = 0; i < REG_NUMBER; i++)
        process->registers[i] = 0;
    process->registers[0] = champ->number;
    process->carry = false;
    process->wait_cycles = 0;
    process->current_op = -1;
    process->last_live_cycle = vm->cycle_counter;
    process->alive = true;
    process->next = vm->processes;
    process->vm = vm;
    vm->processes = process;
    vm->process_count++;
}

void init_decode_context(decode_context_t *ctx, vm_t *vm,
    process_t *process, instruction_t *instruction)
{
    ctx->vm = vm;
    ctx->pc = process->pc;
    ctx->offset = 0;
    ctx->opcode = instruction->op_code;
    ctx->inst = instruction;
    ctx->offset = 1;
}

champion_t *init_champion(void)
{
    champion_t *champ = malloc(sizeof(champion_t));

    if (!champ)
        return NULL;
    champ->number = -1;
    champ->load_address = -1;
    champ->alive = true;
    champ->last_live = 0;
    champ->lives_in_period = 0;
    return champ;
}
