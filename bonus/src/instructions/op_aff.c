/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_aff.c
*/

#include "corewar.h"
#include <unistd.h>

void op_aff(process_t *process, byte_t *memory __attribute__((unused)),
    champion_t **champions __attribute__((unused)), int champion_count
    __attribute__((unused)))
{
    int arg1_type = process->current_op_arg_types[0];
    int arg1_value = process->current_op_args[0];
    char c;

    if (arg1_type != T_REG || arg1_value < 1 || arg1_value > REG_NUMBER)
        return;
    c = (get_register_value(process, arg1_value) % 256);
    write(1, &c, 1);
}
