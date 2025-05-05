/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** read_short_int.c
*/

#include "corewar.h"

int read_short(byte_t *memory, int pos)
{
    unsigned short value = (memory[pos % MEM_SIZE] << 8) |
    memory[(pos + 1) % MEM_SIZE];

    if (value & 0x8000)
        return (int)(short)value;
    return value;
}

int read_int(byte_t *memory, int pos)
{
    return (memory[pos % MEM_SIZE] << 24) |
    (memory[(pos + 1) % MEM_SIZE] << 16) |
    (memory[(pos + 2) % MEM_SIZE] << 8) |
    memory[(pos + 3) % MEM_SIZE];
}
