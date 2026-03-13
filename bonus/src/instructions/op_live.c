/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_live.c
*/

#include "corewar.h"
#include <unistd.h>

static void write_live_message(bool champion_found, champion_t *champion,
    process_t *process, int champion_number)
{
    if (champion_found) {
        champion->last_live = process->vm->cycle_counter;
        champion->lives_in_period++;
        process->vm->last_alive_champion = champion_number;
        write(1, "The player ", 11);
        my_putnbr(champion_number);
        write(1, "(", 1);
        write(1, champion->name, my_strlen(champion->name));
        write(1, ") is alive.\n", 12);
    }
}

void op_live(process_t *process, byte_t *memory __attribute__((unused)),
    champion_t **champions, int champion_count)
{
    int champion_number = process->current_op_args[0];
    champion_t *champion = NULL;
    bool champion_found = false;

    if (process->current_op_arg_types[0] != T_DIR)
        return;
    process->last_live_cycle = process->vm->cycle_counter;
    process->vm->lives_in_period++;
    for (int i = 0; i < champion_count; i++) {
        if (champions[i]->number == champion_number) {
            champion = champions[i];
            champion_found = true;
            break;
        }
    }
    write_live_message(champion_found, champion, process, champion_number);
}
