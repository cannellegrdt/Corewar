/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** json_output.c
*/

#include "corewar.h"
#include <stdio.h>

static void print_memory_json(vm_t *vm)
{
    printf("  \"memory\": {");
    bool first_mem = true;
    for (int i = 0; i < MEM_SIZE; i++) {
        if (vm->memory[i] != 0) {
            if (!first_mem) printf(",");
            printf("\n    \"%d\": %d", i, vm->memory[i]);
            first_mem = false;
        }
    }
    if (!first_mem) printf("\n  ");
    printf("}");
}

static void print_champions_json(vm_t *vm)
{
    printf("  \"champions\": [");
    for (int i = 0; i < vm->champion_count; i++) {
        if (i > 0) printf(",");
        printf("\n    {");
        printf("\n      \"number\": %d,", vm->champions[i]->number);
        printf("\n      \"name\": \"%s\",", vm->champions[i]->name);
        printf("\n      \"alive\": %s,", vm->champions[i]->alive ? "true" : "false");
        printf("\n      \"last_live\": %d,", vm->champions[i]->last_live);
        printf("\n      \"lives_in_period\": %d,", vm->champions[i]->lives_in_period);
        printf("\n      \"load_address\": %d", vm->champions[i]->load_address);
        printf("\n    }");
    }
    printf("\n  ]");
}

static void print_processes_json(vm_t *vm)
{
    printf("  \"processes\": [");
    process_t *current = vm->processes;
    bool first_proc = true;
    while (current) {
        if (!first_proc) printf(",");
        printf("\n    {");
        printf("\n      \"champion_number\": %d,", current->champion_number);
        printf("\n      \"pc\": %d,", current->pc);
        printf("\n      \"carry\": %s,", current->carry ? "true" : "false");
        printf("\n      \"alive\": %s,", current->alive ? "true" : "false");
        printf("\n      \"last_live_cycle\": %d", current->last_live_cycle);
        printf("\n    }");
        first_proc = false;
        current = current->next;
    }
    printf("\n  ]");
}

void print_json_state(vm_t *vm)
{
    if (!vm || !vm->options.json_output)
        return;
        
    printf("{\n");
    printf("  \"cycle\": %d,\n", vm->cycle_counter);
    printf("  \"cycle_to_die\": %d,\n", vm->cycle_to_die);
    printf("  \"last_alive_champion\": %d,\n", vm->last_alive_champion);
    
    print_memory_json(vm);
    printf(",\n");
    print_champions_json(vm);
    printf(",\n");
    print_processes_json(vm);
    
    printf("\n}\n");
    fflush(stdout);
}