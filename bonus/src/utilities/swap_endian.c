/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** swap_endian.c
*/

#include "corewar.h"

int swap_endian(int value)
{
    return ((value & 0xFF) << 24) |
    ((value & 0xFF00) << 8) |
    ((value & 0xFF0000) >> 8) |
    ((value & 0xFF000000) >> 24);
}
