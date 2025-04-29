/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** parse_arguments.c
*/

#include "corewar.h"
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int swap_endian(int value)
{
    return ((value & 0xFF) << 24) |
    ((value & 0xFF00) << 8) |
    ((value & 0xFF0000) >> 8) |
    ((value & 0xFF000000) >> 24);
}

int check_overlap(vm_t *vm)
{
    for (int i =0; i < vm->champion_count; i++) {
        champion_t *champ1 = vm->champions[i];
        int start1 = champ1->load_address;
        int end1 = (start1 + champ1->size - 1) % MEM_SIZE;
        for (int j = i + 1; j < vm->champion_count; j++) {
            champion_t *champ2 = vm->champions[j];
            int start2 = champ2->load_address;
            int end2 = (start2 + champ2->size - 1) % MEM_SIZE;
            if ((start1 <= end2 && end1 >= start2) ||
            (start2 <= end1 && end2 >= start1))
                return error_msg("Error: champions overlap in memory.\n", 84);
        }
    }
    return 0;
}

void free_champions(champion_t **champions, int count)
{
    if (!champions)
        return;
    for (int i = 0; i < count; i++) {
        if (champions[i]) {
            if (champions[i]->code)
                free(champions[i]->code);
            if (champions[i]->filename)
                free(champions[i]->filename);
            free(champions[i]);
        }
    }
    free(champions);
}

void free_vm(vm_t *vm)
{
    free_champions(vm->champions, vm->champion_count);
    process_t *current = vm->processes;
    while (current) {
        process_t *next = current->next;
        free(current);
        current = next;
    }
}

void initialize_vm(vm_t *vm)
{
    my_memset(vm->memory, 0, MEM_SIZE);
    vm->champions = NULL;
    vm->processes = 0;
    vm->process_count = 0;
    vm->cycle_counter = 0;
    vm->cycle_to_die = CYCLE_TO_DIE;
    vm->checks_performed = 0;
    vm->lives_in_period = 0;
    vm->last_alive_champion = 0;
    vm->options.dump_flag = false;
    vm->options.dump_cycle = 0;

    //vm->op_functions[0] = &op_live;
    //...
}

void create_process(vm_t *vm, champion_t *champ)
{
    process_t *process = malloc(sizeof(process_t));

    if (!process)
        return;
    process->champion_number = champ->number;
    process->pc = champ->load_address;
    my_memset(process->registers, 0, sizeof(int) * REG_NUMBER);
    process->registers[0] = champ->number;
    process->carry = false;
    process->wait_cycles = 0;
    process->current_op = -1;
    process->alive = true;
    process->next = vm->processes;
    vm->processes = process;
    vm->process_count++;
}

int read_champion_file(champion_t *champ)
{
    int fd = open(champ->filename, O_RDONLY);

    if (fd == -1)
        return error_msg("Error: cannot open champion file.\n", 84);
    header_t header;
    if (read(fd, &header, sizeof(header_t)) != sizeof(header_t)) {
        close(fd);
        return error_msg("Error: invalid champion file.\n", 84);
    }
    if (swap_endian(header.magic) != COREWAR_EXEC_MAGIC) {
        close(fd);
        return error_msg("Error: invalid magic number.\n", 84);
    }
    my_strncpy(champ->name, header.prog_name, PROG_NAME_LENGTH);
    champ->name[PROG_NAME_LENGTH] = '\0';
    my_strncpy(champ->comment, header.comment, COMMENT_LENGTH);
    champ->comment[COMMENT_LENGTH] = '\0';
    champ->size = swap_endian(header.prog_size);
    champ->code = malloc(champ->size);
    if (!champ->code) {
        close(fd);
        return error_msg("Error: memory allocation failed.\n", 84);
    }
    if (read(fd, champ->code, champ->size) != champ->size) {
        close(fd);
        free(champ->code);
        return error_msg("Error: invalid champion file.\n", 84);
    }
    return 0;
}

void calculate_load_addresses(vm_t *vm)
{
    int spacing = MEM_SIZE / vm->champion_count;

    for (int i = 0; i < vm->champion_count; i++) {
        if (vm->champions[i]->load_address == -1)
            vm->champions[i]->load_address = (i * spacing) % MEM_SIZE;
    }
    check_overlap(vm);
}

int load_champions(vm_t *vm)
{
    calculate_load_addresses(vm);
    for (int i = 0; i < vm->champion_count; i++) {
        champion_t *champ = vm->champions[i];
        if (read_champion_file(champ) != 0)
            return 84;
        for (int j = 0; j < champ->size; j++)
            vm->memory[(champ->load_address + j) % MEM_SIZE] = champ->code[j];
        create_process(vm, champ);
    }
    return 0;
}

int parse_arguments(int argc, char *argv[])
{
    int i = 1;
    vm_t vm;

    initialize_vm(&vm);
    if (i < argc && !my_strcmp(argv[i], "-dump")) {
        if (i + 1 >= argc)
            return error_msg("Error: -dump requires a cycle number.\n", 84);
        vm.options.dump_flag = true;
        vm.options.dump_cycle = my_getnbr(argv[i + 1]);
        if (vm.options.dump_cycle <= 0)
            return error_msg("Error: dump cycle must be positive.\n", 84);
        i += 2;
    }

    champion_t **champs = malloc(sizeof(champion_t *) * ((argc - i) / 2 + 1));
    if (!champs)
        return error_msg("Error: memory allocation failed.\n", 84);
    int champ_count = 0;
    int next_number = 1;

    while (i < argc) {
        champion_t *champ = malloc(sizeof(champion_t));

        if (!champ) {
            free_champions(champs, champ_count);
            return error_msg("Error: memory allocation failed.\n", 84);
        }
        champ->number = -1;
        champ->load_address = -1;
        champ->alive = true;
        champ->last_live = 0;
        champ->lives_in_period = 0;
        while (i < argc && (argv[i][0] == '-')) {
            if (!my_strcmp(argv[i], "-n")) {
                if (i + 1 >= argc)
                    return error_msg("Error: -n requires a number.\n", 84);
                champ->number = my_getnbr(argv[i + 1]);
                if (champ->number <= 0)
                    return error_msg("Error: champion number must be positive.\n", 84);
                for (int j = 0; j < champ_count; j++) {
                    if (champs[j]->number == champ->number)
                        return error_msg("Error: duplicate champion number.\n", 84);
                }
                i += 2;
            } else if (!my_strcmp(argv[i], "-a")) {
                if (i + 1 >= argc)
                    return error_msg("Error: -a requires an address.\n", 84);
                champ->load_address = my_getnbr(argv[i + 1]) % MEM_SIZE;
                if (champ->load_address < 0)
                    champ->load_address += MEM_SIZE;
                i += 2;
            } else
                break;
        }
        if (i >= argc)
            return error_msg("Error: champion filename expected.\n", 84);
        champ->filename = my_strdup(argv[i]);
        i++;
        if (champ->number == -1) {
            champ->number = next_number;
            while (1) {
                bool used = false;
                for (int j = 0; j < champ_count; j++) {
                    if (champs[j]->number == next_number) {
                        used = true;
                        break;
                    }
                }
                if (!used)
                    break;
                next_number++;
            }
            next_number++;
        }
        champs[champ_count] = champ;
        champ_count++;
    }
    if (champ_count == 0) {
        free(champs);
        return error_msg("Error: no champions provided.\n", 84);
    }
    vm.champions = champs;
    vm.champion_count = champ_count;
    if (load_champions(&vm)) {
        free_vm(&vm);
        return 84;
    }
    //run_vm(&vm);
    free_vm(&vm);
    return 0;
}
