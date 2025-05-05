/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** load_address.c
*/

#include "corewar.h"

int calculate_address(int base_address, int offset, bool apply_idx_mod)
{
    int address;

    if (apply_idx_mod)
        offset = offset % IDX_MOD;
    address = base_address + offset;
    address = address % MEM_SIZE;
    if (address < 0)
        address += MEM_SIZE;
    return address;
}
