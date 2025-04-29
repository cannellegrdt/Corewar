/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** main.c
*/

#include "corewar.h"

int main(int argc, char **argv)
{
    if (argc == 2 && (!my_strcmp(argv[1], "--help") ||
    (!my_strcmp(argv[1], "-h"))))
        return print_help();
    if (argc == 1)
        return error_msg("Error: at least two champions are needed.\n", 84);
    parse_arguments(argc, argv);
    return 0;
}
