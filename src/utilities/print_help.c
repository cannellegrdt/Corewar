/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** print_help.c
*/

#include "corewar.h"
#include <unistd.h>

int print_help(void)
{
    write(1, "USAGE\n", 6);
    write(1, "    ./corewar [-dump nbr_cycle] [[-n prog_number] "
    "[-a load_address] prog_name] ...\n", 83);
    write(1, "\nDESCRIPTION\n", 13);
    write(1, "    -dump nbr_cycle   Dumps the memory after the nbr_cycle "
    "execution (if the round isn't\n                      already over) with "
    "the following format: 32 bytes/line in hexadecimal (A0BCDEFE1DD3...)\n",
    199);
    write(1, "\n", 1);
    write(1, "    -n prog_number    Sets the next program's number. "
    "By default, the first free number in the parameter order\n", 111);
    write(1, "\n", 1);
    write(1, "    -a load_address   Sets the next program's loading "
    "address.\n                      When no address is specified, optimize "
    "the addresses so that the processes are as far away from \n             "
    "         each other as possible. The addresses are MEM_SIZE modulo.\n",
    263);
    return 0;
}
