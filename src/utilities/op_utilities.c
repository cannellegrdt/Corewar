/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** op_utilities.c
*/

#include "corewar.h"

int get_register_value(process_t *process, int reg_num)
{
    if (reg_num < 1 || reg_num > REG_NUMBER)
        return 0;
    return process->registers[reg_num - 1];
}

bool set_register_value(process_t *process, int reg_num, int value)
{
    if (reg_num < 1 || reg_num > REG_NUMBER)
        return false;
    process->registers[reg_num - 1] = value;
    return true;
}

int read_memory(byte_t *memory, int address, int size)
{
    int value = 0;

    address = address % MEM_SIZE;
    if (address < 0)
        address += MEM_SIZE;
    for (int i = 0; i < size; i++)
        value = (value << 8) | memory[(address + i) % MEM_SIZE];
    return value;
}

void write_memory(byte_t *memory, int address, int value, int size)
{
    address = address % MEM_SIZE;
    if (address < 0)
        address += MEM_SIZE;
    for (int i = size - 1; i >= 0; i--) {
        memory[(address + i) % MEM_SIZE] = value & 0xFF;
        value >>= 8;
    }
}

int get_param_value(get_param_value_args_t args)
{
    int address;

    switch (args.param_type) {
        case T_REG:
            return get_register_value(args.process, args.param_value);
        case T_DIR:
            return args.param_value;
        case T_IND:
            address = calculate_address(args.process->pc,
                args.param_value, args.is_modulo);
            return read_memory(args.memory, address, DIR_SIZE);
        default:
            return 0;
    }
}
