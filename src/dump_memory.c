/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** dump_memory.c
*/

#include "corewar.h"
#include <unistd.h>

void print_hex_byte(byte_t byte)
{
    char hex_chars[] = "0123456789ABCDEF";
    char hex[3];

    hex[0] = hex_chars[(byte >> 4) & 0xF];
    hex[1] = hex_chars[byte & 0xF];
    hex[2] = '\0';
    write(1, hex, 2);
}

void dump_memory(byte_t *memory)
{
    for (int i = 0; i < MEM_SIZE; i++) {
        if (i % 32 == 0 && i > 0)
            write(1, "\n", 1);
        print_hex_byte(memory[i]);
        if ((i + 1) % 32 != 0)
            write(1, " ", 1);
    }
    write(1, "\n", 1);
}
