/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** unit_tests.c
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "corewar.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(string_utils, test_my_strcmp)
{
    cr_assert_eq(my_strcmp("hello", "hello"), 0);
    cr_assert_neq(my_strcmp("hello", "world"), 0);
    cr_assert_eq(my_strcmp("", ""), 0);
    cr_assert_neq(my_strcmp("hello", ""), 0);
    cr_assert_neq(my_strcmp("", "world"), 0);
    cr_assert_gt(my_strcmp("hello", "hell"), 0);
    cr_assert_eq(my_strcmp("hell", "hello"), 1);
}

Test(string_utils, test_my_strlen)
{
    cr_assert_eq(my_strlen("hello"), 5);
    cr_assert_eq(my_strlen(""), 0);
    cr_assert_eq(my_strlen("h"), 1);
    cr_assert_eq(my_strlen(NULL), 0);
}

Test(string_utils, test_my_strncpy)
{
    char dest[10];
    
    memset(dest, 0, 10);
    my_strncpy(dest, "hello", 5);
    cr_assert_str_eq(dest, "hello");
    
    memset(dest, 0, 10);
    my_strncpy(dest, "hello world", 5);
    cr_assert_str_eq(dest, "hello");
    
    memset(dest, 0, 10);
    my_strncpy(dest, "hi", 5);
    cr_assert_str_eq(dest, "hi");
}

Test(string_utils, test_my_strdup)
{
    char *dup = my_strdup("hello");
    cr_assert_str_eq(dup, "hello");
    free(dup);
    
    dup = my_strdup("");
    cr_assert_str_eq(dup, "");
    free(dup);
}

Test(string_utils, test_endswith)
{
    cr_assert(endswith("test.cor", ".cor"));
    cr_assert_not(endswith("test.s", ".cor"));
    cr_assert_not(endswith("test", ".cor"));
    cr_assert(endswith(".cor", ".cor"));
    cr_assert_not(endswith("cor", ".cor"));
}

Test(conversion_utils, test_my_getnbr)
{
    cr_assert_eq(my_getnbr("42"), 42);
    cr_assert_eq(my_getnbr("-42"), -42);
    cr_assert_eq(my_getnbr("0"), 0);
    cr_assert_eq(my_getnbr("  42"), 42);
    cr_assert_eq(my_getnbr("  -42"), -42);
    cr_assert_eq(my_getnbr("42abc"), 42);
}

Test(conversion_utils, test_swap_endian)
{
    cr_assert_eq(swap_endian(0x12345678), 0x78563412);
    cr_assert_eq(swap_endian(0x00000001), 0x01000000);
    cr_assert_eq(swap_endian(0xFFFFFFFF), 0xFFFFFFFF);
}

Test(memory_utils, test_my_memset)
{
    char buffer[10];
    
    my_memset(buffer, 'A', 10);
    for (int i = 0; i < 10; i++)
        cr_assert_eq(buffer[i], 'A');
    
    my_memset(buffer, 0, 10);
    for (int i = 0; i < 10; i++)
        cr_assert_eq(buffer[i], 0);
    
    cr_assert_null(my_memset(NULL, 'A', 10));
}

Test(memory_utils, test_read_short)
{
    byte_t memory[MEM_SIZE] = {0};
    
    memory[0] = 0x12;
    memory[1] = 0x34;
    cr_assert_eq(read_short(memory, 0), 0x1234);
}

Test(memory_utils, test_read_int)
{
    byte_t memory[MEM_SIZE] = {0};
    
    memory[0] = 0x12;
    memory[1] = 0x34;
    memory[2] = 0x56;
    memory[3] = 0x78;
    cr_assert_eq(read_int(memory, 0), 0x12345678);
    
    memory[MEM_SIZE-4] = 0xAA;
    memory[MEM_SIZE-3] = 0xBB;
    memory[MEM_SIZE-2] = 0xCC;
    memory[MEM_SIZE-1] = 0xDD;
    cr_assert_eq(read_int(memory, MEM_SIZE-4), 0xAABBCCDD);
}

Test(memory_utils, test_calculate_address)
{
    cr_assert_eq(calculate_address(100, 50, true), 150);
    
    cr_assert_eq(calculate_address(100, 600, true), 100 + (600 % IDX_MOD));
    
    cr_assert_eq(calculate_address(100, 600, false), 700);
    
    cr_assert_eq(calculate_address(100, -50, true), 50);
    
    cr_assert_eq(calculate_address(MEM_SIZE - 10, 20, false), 10);
    
    cr_assert_eq(calculate_address(10, -20, false), MEM_SIZE - 10);
}

Test(memory_utils, test_read_write_memory)
{
    byte_t memory[MEM_SIZE] = {0};
    
    write_memory(memory, 100, 0x12345678, 4);
    cr_assert_eq(read_memory(memory, 100, 4), 0x12345678);
    
    write_memory(memory, MEM_SIZE - 2, 0xAABB, 2);
    cr_assert_eq(read_memory(memory, MEM_SIZE - 2, 2), 0xAABB);
    
    write_memory(memory, 200, 0x12345678, 4);
    cr_assert_eq(read_memory(memory, 200, 2), 0x1234);
}

Test(decode_utils, test_get_param_type)
{
    decode_context_t ctx;
    
    ctx.has_coding_byte = false;
    ctx.opcode = 1;
    cr_assert_eq(get_param_type(&ctx, 0), T_DIR);
    
    ctx.opcode = 10;
    cr_assert_eq(get_param_type(&ctx, 0), 0);
    
    ctx.has_coding_byte = true;
    ctx.coding_byte = 0x54;
    
    cr_assert_eq(get_param_type(&ctx, 0), T_REG);
    cr_assert_eq(get_param_type(&ctx, 2), T_REG);
    cr_assert_eq(get_param_type(&ctx, 3), 0);
}

Test(champion_utils, test_init_champion)
{
    champion_t *champ = init_champion();
    
    cr_assert_not_null(champ);
    cr_assert_eq(champ->number, -1);
    cr_assert_eq(champ->load_address, -1);
    cr_assert(champ->alive);
    cr_assert_eq(champ->last_live, 0);
    cr_assert_eq(champ->lives_in_period, 0);
    
    free(champ);
}

Test(output_utils, test_error_msg, .init = redirect_all_std)
{
    int result = error_msg("Test error", 42);
    
    cr_assert_stderr_eq_str("Test error");
    cr_assert_eq(result, 42);
}

Test(output_utils, test_print_help, .init = redirect_all_std)
{
    int result = print_help();
    
    cr_assert_stdout_neq_str("");
    cr_assert_eq(result, 0);
}

Test(process_utils, test_get_set_register_value)
{
    process_t process;
    
    for (int i = 0; i < REG_NUMBER; i++)
        process.registers[i] = 0;
    
    cr_assert(set_register_value(&process, 1, 42));
    cr_assert_eq(get_register_value(&process, 1), 42);
    
    cr_assert(set_register_value(&process, REG_NUMBER, 123));
    cr_assert_eq(get_register_value(&process, REG_NUMBER), 123);
    
    cr_assert_not(set_register_value(&process, 0, 42));
    cr_assert_eq(get_register_value(&process, 0), 0);
    
    cr_assert_not(set_register_value(&process, REG_NUMBER + 1, 42));
    cr_assert_eq(get_register_value(&process, REG_NUMBER + 1), 0);
}

Test(validation_utils, test_verify_validity_arg)
{
    cr_assert_eq(verify_validity_arg("123"), 0);
    cr_assert_eq(verify_validity_arg("+123"), 0);
    
    cr_assert_eq(verify_validity_arg("-123"), 84);
    cr_assert_eq(verify_validity_arg(""), 84);
    cr_assert_eq(verify_validity_arg("abc"), 84);
    cr_assert_eq(verify_validity_arg("123abc"), 84);
}

Test(validation_utils, test_is_number_used)
{
    champion_t champ1, champ2;
    champion_t *champs[2];
    
    champ1.number = 1;
    champ2.number = 2;
    champs[0] = &champ1;
    champs[1] = &champ2;
    
    cr_assert(is_number_used(1, 2, champs));
    cr_assert(is_number_used(2, 2, champs));
    cr_assert_not(is_number_used(3, 2, champs));
    cr_assert_not(is_number_used(0, 2, champs));
}

Test(validation_utils, test_find_available_number)
{
    champion_t champ1, champ2;
    champion_t *champs[2];
    
    champ1.number = 1;
    champ2.number = 2;
    champs[0] = &champ1;
    champs[1] = &champ2;
    
    cr_assert_eq(find_available_number(1, 2, champs), 3);
    cr_assert_eq(find_available_number(2, 2, champs), 3);
    cr_assert_eq(find_available_number(3, 2, champs), 3);
    cr_assert_eq(find_available_number(4, 2, champs), 4);
}

Test(vm_utils, test_initialize_vm)
{
    vm_t vm;
    
    initialize_vm(&vm);
    
    cr_assert_null(vm.champions);
    cr_assert_eq(vm.champion_count, 0);
    cr_assert_null(vm.processes);
    cr_assert_eq(vm.process_count, 0);
    cr_assert_eq(vm.cycle_counter, 0);
    cr_assert_eq(vm.cycle_to_die, CYCLE_TO_DIE);
    cr_assert_eq(vm.checks_performed, 0);
    cr_assert_eq(vm.lives_in_period, 0);
    cr_assert_eq(vm.last_alive_champion, 0);
    cr_assert_not(vm.options.dump_flag);
    cr_assert_eq(vm.options.dump_cycle, 0);
    
    for (int i = 0; i < 16; i++)
        cr_assert_not_null(vm.op_functions[i]);
}

Test(decode_args, test_read_coding_byte)
{
    vm_t vm;
    process_t process;
    decode_context_t ctx;
    
    memset(&vm, 0, sizeof(vm_t));
    memset(&process, 0, sizeof(process_t));
    
    vm.memory[1] = 0x54;
    ctx.vm = &vm;
    ctx.pc = 0;
    ctx.offset = 1;
    ctx.opcode = 2;
    
    cr_assert(read_coding_byte(&ctx));
    cr_assert(ctx.has_coding_byte);
    cr_assert_eq(ctx.coding_byte, 0x54);
    cr_assert_eq(ctx.offset, 2);
    
    ctx.offset = 1;
    ctx.opcode = 1;
    
    cr_assert(read_coding_byte(&ctx));
    cr_assert_not(ctx.has_coding_byte);
    cr_assert_eq(ctx.coding_byte, 0);
    cr_assert_eq(ctx.offset, 1);
}

Test(sort_processes, test_sort_processes_by_champion_number)
{
    vm_t vm;
    process_t proc1, proc2, proc3;
    
    initialize_vm(&vm);
    
    memset(&proc1, 0, sizeof(process_t));
    memset(&proc2, 0, sizeof(process_t));
    memset(&proc3, 0, sizeof(process_t));
    
    proc1.champion_number = 3;
    proc2.champion_number = 1;
    proc3.champion_number = 2;
    
    proc1.next = &proc2;
    proc2.next = &proc3;
    proc3.next = NULL;
    
    vm.processes = &proc1;
    vm.process_count = 3;
    
    sort_processes_by_champion_number(&vm);
    
    cr_assert_eq(vm.processes->champion_number, 1);
    cr_assert_eq(vm.processes->next->champion_number, 2);
    cr_assert_eq(vm.processes->next->next->champion_number, 3);
    cr_assert_null(vm.processes->next->next->next);
}

Test(process_creation, test_create_process)
{
    vm_t vm;
    champion_t champ;
    
    initialize_vm(&vm);
    memset(&champ, 0, sizeof(champion_t));
    
    champ.number = 42;
    champ.load_address = 100;
    
    create_process(&vm, &champ);
    
    cr_assert_not_null(vm.processes);
    cr_assert_eq(vm.process_count, 1);
    cr_assert_eq(vm.processes->champion_number, 42);
    cr_assert_eq(vm.processes->pc, 100);
    cr_assert_eq(vm.processes->registers[0], 42);
    for (int i = 1; i < REG_NUMBER; i++)
        cr_assert_eq(vm.processes->registers[i], 0);
    cr_assert_not(vm.processes->carry);
    cr_assert_eq(vm.processes->wait_cycles, 0);
    cr_assert_eq(vm.processes->current_op, -1);
    cr_assert_eq(vm.processes->last_live_cycle, 0);
    cr_assert(vm.processes->alive);
    cr_assert_null(vm.processes->next);
}

Test(lifecycle_utils, test_check_lives)
{
    vm_t vm;
    process_t process;
    champion_t champion;
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    memset(&champion, 0, sizeof(champion_t));
    
    champion.number = 1;
    champion.alive = true;
    champion.lives_in_period = 5;
    
    vm.champions = malloc(sizeof(champion_t *));
    vm.champions[0] = &champion;
    vm.champion_count = 1;
    
    process.champion_number = 1;
    process.alive = true;
    process.last_live_cycle = vm.cycle_counter;
    process.next = NULL;
    
    vm.processes = &process;
    vm.process_count = 1;
    vm.cycle_to_die = CYCLE_TO_DIE;
    vm.lives_in_period = 45;
    
    check_lives(&vm);
    
    cr_assert_eq(vm.cycle_to_die, CYCLE_TO_DIE - CYCLE_DELTA);
    cr_assert_eq(vm.lives_in_period, 0);
    cr_assert_eq(champion.lives_in_period, 0);
    cr_assert_eq(vm.process_count, 1);
    
    process.last_live_cycle = 0;
    vm.cycle_counter = CYCLE_TO_DIE + 1;
    vm.lives_in_period = 10;
    free(vm.champions);
}

Test(op_functions, test_op_live, .init = redirect_all_std)
{
    vm_t vm;
    process_t process;
    champion_t champion;
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    memset(&champion, 0, sizeof(champion_t));
    
    champion.number = 42;
    strcpy(champion.name, "Test Champion");
    champion.alive = true;
    
    vm.champions = malloc(sizeof(champion_t *));
    vm.champions[0] = &champion;
    vm.champion_count = 1;
    
    process.vm = &vm;
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_args[0] = 42;
    
    op_live(&process, vm.memory, vm.champions, vm.champion_count);
    
    cr_assert_eq(vm.last_alive_champion, 42);
    cr_assert_eq(vm.lives_in_period, 1);
    cr_assert_eq(champion.lives_in_period, 1);
    
    cr_assert_stdout_eq_str("The player 42(Test Champion) is alive.\n");
    
    free(vm.champions);
}

Test(champion_loading, test_calculate_load_addresses)
{
    vm_t vm;
    champion_t champ1, champ2, champ3;
    
    initialize_vm(&vm);
    memset(&champ1, 0, sizeof(champion_t));
    memset(&champ2, 0, sizeof(champion_t));
    memset(&champ3, 0, sizeof(champion_t));
    
    champ1.load_address = -1;
    champ2.load_address = -1;
    champ3.load_address = 100;
    
    vm.champions = malloc(3 * sizeof(champion_t *));
    vm.champions[0] = &champ1;
    vm.champions[1] = &champ2;
    vm.champions[2] = &champ3;
    vm.champion_count = 3;
    
    calculate_load_addresses(&vm);
    
    int spacing = MEM_SIZE / 3;
    cr_assert_eq(champ1.load_address, 0 * spacing);
    cr_assert_eq(champ2.load_address, 1 * spacing);
    cr_assert_eq(champ3.load_address, 100);
    
    free(vm.champions);
}

Test(op_functions, test_op_add)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    memset(&process, 0, sizeof(process_t));
    
    process.registers[0] = 10;
    process.registers[1] = 20;
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 1;
    process.current_op_args[1] = 2;
    process.current_op_args[2] = 3;

    op_add(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 30);
    cr_assert_not(process.carry);
    
    process.registers[0] = 0;
    process.registers[1] = 0;
    
    op_add(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0);
    cr_assert(process.carry);
    
    process.current_op_args[0] = 0;
    
    op_add(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0);
    cr_assert(process.carry);
}

Test(op_functions, test_op_or)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    memset(&process, 0, sizeof(process_t));
    
    process.registers[0] = 0x0F;
    process.registers[1] = 0xF0;
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 1;
    process.current_op_args[1] = 2;
    process.current_op_args[2] = 3;
    
    op_or(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0xFF);
    cr_assert_not(process.carry);
    
    process.registers[0] = 0;
    process.registers[1] = 0;
    
    op_or(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0);
    cr_assert(process.carry);
}

Test(op_functions, test_op_xor)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    memset(&process, 0, sizeof(process_t));
    
    process.registers[0] = 0xFF;
    process.registers[1] = 0xF0;
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 1;
    process.current_op_args[1] = 2;
    process.current_op_args[2] = 3;
    
    op_xor(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0x0F);
    cr_assert_not(process.carry);
    
    process.registers[0] = 0xAA;
    process.registers[1] = 0xAA;
    
    op_xor(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0);
    cr_assert(process.carry);
}

Test(op_functions, test_op_zjmp)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 100;
    
    process.carry = false;
    process.current_op_args[0] = 50;
    
    op_zjmp(&process, memory, NULL, 0);
    
    cr_assert_eq(process.pc, 100);
    
    process.carry = true;
    process.current_op_args[0] = 50;
    
    op_zjmp(&process, memory, NULL, 0);
    
    cr_assert_eq(process.pc, 150);
    
    process.pc = 100;
    process.current_op_args[0] = -25;
    
    op_zjmp(&process, memory, NULL, 0);
    
    cr_assert_eq(process.pc, 75);
    
    process.pc = MEM_SIZE - 10;
    process.current_op_args[0] = 20;
    
    op_zjmp(&process, memory, NULL, 0);
    
    cr_assert_eq(process.pc, 10);
}

Test(op_functions, test_op_ld)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 100;
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 42;
    process.current_op_args[1] = 1;
    
    op_ld(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[0], 42);
    cr_assert_not(process.carry);

    process.current_op_arg_types[0] = T_IND;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 50;
    process.current_op_args[1] = 2;
    
    write_memory(memory, 150, 0x12345678, REG_SIZE);
    
    op_ld(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[1], 0x12345678);
    cr_assert_not(process.carry);
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 0;
    process.current_op_args[1] = 3;
    
    op_ld(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0);
    cr_assert(process.carry);

    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 99;
    process.current_op_args[1] = 0;
    
    process.carry = false;
    
    op_ld(&process, memory, NULL, 0);
    
    cr_assert_not(process.carry);
}

Test(champion_creation, test_open_champion_file)
{
    int fd = open("test_champion.cor", O_CREAT | O_WRONLY, 0644);
    close(fd);
    
    champion_t champ;
    memset(&champ, 0, sizeof(champion_t));
    champ.filename = strdup("test_champion.cor");
    
    fd = open("test_champion.cor", O_WRONLY | O_TRUNC);
    header_t header;
    memset(&header, 0, sizeof(header_t));
    header.magic = swap_endian(COREWAR_EXEC_MAGIC);
    header.prog_size = swap_endian(4);
    strcpy(header.prog_name, "Test Champion");
    strcpy(header.comment, "Test Comment");
    write(fd, &header, sizeof(header_t));
    char code[4] = {1, 0, 0, 1};
    write(fd, code, 4);
    close(fd);
    
    int result = read_champion_file(&champ);
    cr_assert_eq(result, 0);
    cr_assert_eq(champ.size, 4);
    cr_assert_str_eq(champ.name, "Test Champion");
    cr_assert_str_eq(champ.comment, "Test Comment");
    cr_assert_not_null(champ.code);
    cr_assert_eq(champ.code[0], 1);
    cr_assert_eq(champ.code[3], 1);
    
    free(champ.code);
    free(champ.filename);
    unlink("test_champion.cor");
    
    memset(&champ, 0, sizeof(champion_t));
    champ.filename = strdup("non_existent.cor");
    result = read_champion_file(&champ);
    cr_assert_eq(result, 84);
    
    free(champ.filename);
}

Test(champion_creation, test_read_champion_header)
{
    int fd = open("test_header.cor", O_CREAT | O_WRONLY, 0644);
    
    header_t header;
    memset(&header, 0, sizeof(header_t));
    header.magic = swap_endian(COREWAR_EXEC_MAGIC);
    header.prog_size = swap_endian(10);
    write(fd, &header, sizeof(header_t));
    close(fd);
    
    champion_t champ;
    memset(&champ, 0, sizeof(champion_t));
    champ.filename = strdup("test_header.cor");
    
    truncate("test_header.cor", sizeof(header_t) - 1);
    int result = read_champion_file(&champ);
    cr_assert_eq(result, 84);
    
    fd = open("test_header.cor", O_WRONLY | O_TRUNC);
    header.magic = swap_endian(0x12345678);
    write(fd, &header, sizeof(header_t));
    close(fd);
    
    result = read_champion_file(&champ);
    cr_assert_eq(result, 84);
    
    fd = open("test_header.cor", O_WRONLY | O_TRUNC);
    header.magic = swap_endian(COREWAR_EXEC_MAGIC);
    header.prog_size = swap_endian(CHAMP_MAX_SIZE + 1);
    write(fd, &header, sizeof(header_t));
    close(fd);
    
    result = read_champion_file(&champ);
    cr_assert_eq(result, 84);
    
    free(champ.filename);
    unlink("test_header.cor");
}


Test(decode_module, test_decode_param)
{
    vm_t vm;
    process_t process;
    instruction_t instruction;
    decode_context_t ctx;
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    memset(&instruction, 0, sizeof(instruction_t));
    
    vm.memory[0] = 2; 
    vm.memory[1] = 0x90; 
    vm.memory[2] = 0x00;
    vm.memory[3] = 0x00;
    vm.memory[4] = 0x0A; 
    vm.memory[6] = 0x03; 
    
    instruction.op_code = 2;
    init_decode_context(&ctx, &vm, &process, &instruction);
    process.pc = 0;
    
    read_coding_byte(&ctx);
    int param_index = 0;
    int param_type = get_param_type(&ctx, param_index);
    
    cr_assert_eq(param_type, T_DIR);
    
    int size = decode_instruction(&vm, &process, &instruction);
    
    cr_assert_gt(size, 0);
    cr_assert_eq(instruction.arg_type[0], T_DIR);
    cr_assert_eq(instruction.arg_type[1], T_REG);
}


Test(decode_args_module, test_decode_args)
{
    vm_t vm;
    process_t process;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    process.vm = &vm;
    
    
    process.pc = 0;
    vm.memory[0] = 1; 
    vm.memory[1] = 0x00;
    vm.memory[2] = 0x00;
    vm.memory[3] = 0x00;
    vm.memory[4] = 0x0A; 
    
    int size = decode_args(&vm, &process);
    
    cr_assert_eq(size, 5); 
    cr_assert_eq(process.current_op_arg_types[0], T_DIR);
    cr_assert_eq(process.current_op_args[0], 10);
    
    
    process.pc = 10;
    vm.memory[10] = 2; 
    vm.memory[11] = 0x90; 
    vm.memory[12] = 0x00;
    vm.memory[13] = 0x00;
    vm.memory[14] = 0x0A; 
    vm.memory[16] = 0x03; 
    
    size = decode_args(&vm, &process);
    
    cr_assert_eq(size, 7); 
    cr_assert_eq(process.current_op_arg_types[0], T_DIR);
    cr_assert_eq(process.current_op_arg_types[1], T_REG);
    cr_assert_eq(process.current_op_args[1], 3);
    
    
    process.pc = 20;
    vm.memory[20] = 0xFF; 
    
    size = decode_args(&vm, &process);
    
    cr_assert_eq(size, 1); 
}


Test(decode_args_module, test_decode_args_with_coding_byte)
{
    vm_t vm;
    process_t process;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    
    
    process.pc = 0;
    vm.memory[0] = 4; 
    vm.memory[1] = 0x54; 
    vm.memory[2] = 0x01; 
    vm.memory[3] = 0x02; 
    vm.memory[4] = 0x03; 
    
    const op_t *op = &op_tab[3]; 
    int offset = decode_args_with_coding_byte(&vm, &process, op, 1);
    
    cr_assert_eq(offset, 5); 
    cr_assert_eq(process.current_op_arg_types[0], T_REG);
    cr_assert_eq(process.current_op_arg_types[1], T_REG);
    cr_assert_eq(process.current_op_arg_types[2], T_REG);
    cr_assert_eq(process.current_op_args[0], 1);
    cr_assert_eq(process.current_op_args[1], 2);
    cr_assert_eq(process.current_op_args[2], 3);
    
    
    process.pc = 10;
    vm.memory[10] = 4; 
    vm.memory[11] = 0x94; 
    
    offset = decode_args_with_coding_byte(&vm, &process, op, 1);
    
    cr_assert_eq(offset, 2); 
    
    
    process.pc = 20;
    vm.memory[20] = 4; 
    vm.memory[21] = 0x54; 
    vm.memory[22] = 0x00; 
    
    offset = decode_args_with_coding_byte(&vm, &process, op, 1);
    
    cr_assert_eq(offset, 3); 
    
    
    process.pc = 30;
    vm.memory[30] = 6; 
    vm.memory[31] = 0x74; 
    vm.memory[32] = 0x01; 
    vm.memory[33] = 0x00; 
    vm.memory[34] = 0x0A;
    vm.memory[35] = 0x03; 
    
    op = &op_tab[5]; 
    offset = decode_args_with_coding_byte(&vm, &process, op, 1);
    
    cr_assert_eq(offset, 6);
    cr_assert_eq(process.current_op_arg_types[0], T_REG);
    cr_assert_eq(process.current_op_arg_types[1], T_IND);
    cr_assert_eq(process.current_op_arg_types[2], T_REG);
    cr_assert_eq(process.current_op_args[0], 1);
    cr_assert_eq(process.current_op_args[1], 10);
    cr_assert_eq(process.current_op_args[2], 3);
}


Test(cycle_execution, test_update_process_wait_cycle)
{
    process_t process;
    vm_t vm;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    process.pc = 0;
    
    
    vm.memory[0] = 1; 
    
    
    process.vm = &vm;
    update_wait_cycles_and_load_instructions(&vm);
    
    
    
    vm.processes = &process;
    vm.process_count = 1;
    process.alive = true;
    process.current_op = -1;
    
    update_wait_cycles_and_load_instructions(&vm);
    
    
    cr_assert_eq(process.current_op, 1);
    cr_assert_eq(process.wait_cycles, op_tab[0].nbr_cycles - 1);
}

Test(cycle_execution, test_execute_cycle)
{
    vm_t vm;
    process_t process1, process2;
    
    
    initialize_vm(&vm);
    memset(&process1, 0, sizeof(process_t));
    memset(&process2, 0, sizeof(process_t));
    
    
    process1.champion_number = 2;
    process1.pc = 0;
    process1.alive = true;
    process1.current_op = -1;
    process1.vm = &vm;
    
    process2.champion_number = 1;
    process2.pc = 10;
    process2.alive = true;
    process2.current_op = -1;
    process2.vm = &vm;
    
    
    process1.next = &process2;
    process2.next = NULL;
    
    
    vm.memory[0] = 1; 
    vm.memory[1] = 0;
    vm.memory[2] = 0;
    vm.memory[3] = 0;
    vm.memory[4] = 1; 
    
    vm.memory[10] = 1; 
    vm.memory[11] = 0;
    vm.memory[12] = 0;
    vm.memory[13] = 0;
    vm.memory[14] = 2; 
    
    
    vm.processes = &process1;
    vm.process_count = 2;
    
    
    execute_cycle(&vm);
    
    
    cr_assert_eq(process1.current_op, 1);
    cr_assert_eq(process2.current_op, 1);
}


Test(op_functions, test_op_st)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 0;
    process.registers[0] = 42; 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 1; 
    process.current_op_args[1] = 2; 
    
    op_st(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[1], 42); 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_IND;
    process.current_op_args[0] = 1; 
    process.current_op_args[1] = 5; 
    
    op_st(&process, memory, NULL, 0);
    
    cr_assert_eq(read_memory(memory, 5, 4), 42); 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 0; 
    process.current_op_args[1] = 3; 
    
    process.registers[2] = 0; 
    op_st(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0); 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 1; 
    process.current_op_args[1] = 0; 
    
    op_st(&process, memory, NULL, 0);
    
}

Test(op_functions, test_op_sti)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 0;
    process.registers[0] = 42; 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_DIR;
    process.current_op_args[0] = 1; 
    process.current_op_args[1] = 5; 
    process.current_op_args[2] = 10; 
    
    op_sti(&process, memory, NULL, 0);
    
    cr_assert_eq(read_memory(memory, 15, 4), 42); 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_DIR;
    process.current_op_args[0] = 0; 
    process.current_op_args[1] = 5;
    process.current_op_args[2] = 10;
    
    
    memset(memory, 0, MEM_SIZE);
    
    op_sti(&process, memory, NULL, 0);
    
    cr_assert_eq(read_memory(memory, 15, 4), 0); 
}

Test(op_functions, test_op_sub)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    
    memset(&process, 0, sizeof(process_t));
    process.registers[0] = 30; 
    process.registers[1] = 10; 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 1; 
    process.current_op_args[1] = 2; 
    process.current_op_args[2] = 3; 
    
    op_sub(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 20); 
    cr_assert_not(process.carry); 
    
    
    process.registers[0] = 10; 
    process.registers[1] = 10; 
    
    op_sub(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0); 
    cr_assert(process.carry); 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 0; 
    process.current_op_args[1] = 2;
    process.current_op_args[2] = 3;
    
    process.carry = false;
    
    op_sub(&process, memory, NULL, 0);
    
    cr_assert_not(process.carry); 
}

Test(op_functions, test_op_lld)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 0;
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 42; 
    process.current_op_args[1] = 1;  
    
    op_lld(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[0], 42); 
    cr_assert_not(process.carry); 
    
    
    
    write_memory(memory, 5, 123, 4);
    
    process.current_op_arg_types[0] = T_IND;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 5;  
    process.current_op_args[1] = 2;  
    
    op_lld(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[1], 123); 
    cr_assert_not(process.carry); 
    
    
    write_memory(memory, 10, 0, 4);
    
    process.current_op_arg_types[0] = T_IND;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_args[0] = 10; 
    process.current_op_args[1] = 3;  
    
    op_lld(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0); 
    cr_assert(process.carry); 
}

Test(op_functions, test_op_lldi)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 0;
    
    
    write_memory(memory, 15, 123, 4);
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 5;  
    process.current_op_args[1] = 10; 
    process.current_op_args[2] = 1;  
    
    op_lldi(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[0], 123); 
    cr_assert_not(process.carry); 
    
    
    process.registers[0] = 5; 
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 1;  
    process.current_op_args[1] = 10; 
    process.current_op_args[2] = 2;  
    
    op_lldi(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[1], 123); 
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 5;
    process.current_op_args[1] = 10;
    process.current_op_args[2] = 0; 
    
    op_lldi(&process, memory, NULL, 0);
    
}

Test(op_functions, test_op_fork)
{
    vm_t vm;
    process_t process;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    process.pc = 100;
    process.champion_number = 1;
    process.registers[0] = 42;
    process.vm = &vm;
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_args[0] = 50; 
    
    
    op_fork(&process, NULL, NULL, 0);
    
    
    cr_assert_not_null(vm.processes);
    cr_assert_eq(vm.process_count, 1);
    cr_assert_eq(vm.processes->pc, 150); 
    cr_assert_eq(vm.processes->champion_number, 1);
    cr_assert_eq(vm.processes->registers[0], 42);
    cr_assert_null(vm.processes->next);
    
    
    free(vm.processes);
}

Test(op_functions, test_op_lfork)
{
    vm_t vm;
    process_t process;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    process.pc = 100;
    process.champion_number = 1;
    process.registers[0] = 42;
    process.vm = &vm;
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_args[0] = MEM_SIZE + 50; 
    
    
    op_lfork(&process, NULL, NULL, 0);
    
    
    cr_assert_not_null(vm.processes);
    cr_assert_eq(vm.process_count, 1);
    cr_assert_eq(vm.processes->pc, 150); 
    cr_assert_eq(vm.processes->champion_number, 1);
    cr_assert_eq(vm.processes->registers[0], 42);
    cr_assert_null(vm.processes->next);
    
    
    free(vm.processes);
}


Test(parser, test_parse_a_flag)
{
    parse_n_flag_args_t args;
    champion_t champ;
    int i = 0;
    char *argv[] = {"-a", "100", "filename.cor"};
    int argc = 3;
    
    
    memset(&champ, 0, sizeof(champion_t));
    champ.load_address = -1;
    
    args.i = &i;
    args.argc = argc;
    args.argv = argv;
    args.champ = &champ;
    args.champ_count = 0;
    args.champs = NULL;
    
    
    int result = parse_a_flag(&args);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(champ.load_address, 100);
    cr_assert_eq(*args.i, 2);

    
    i = 0;
    argc = 3;
    argv[1] = "abc";
    result = parse_a_flag(&args);
    
    cr_assert_eq(result, 84);
    
    
    i = 0;
    argv[1] = "-1";
    result = parse_a_flag(&args);
    
    cr_assert_eq(result, 84);
    
    
    i = 0;
    argv[1] = "999999";
    result = parse_a_flag(&args);
    
    cr_assert_eq(result, 84);
}

Test(parser, test_parse_n_flag)
{
    parse_n_flag_args_t args;
    champion_t champ, champ2;
    champion_t *champs[2];
    int i = 0;
    char *argv[] = {"-n", "5", "filename.cor"};
    int argc = 3;
    
    
    memset(&champ, 0, sizeof(champion_t));
    memset(&champ2, 0, sizeof(champion_t));
    champ.number = -1;
    champ2.number = 3;
    champs[0] = &champ2;
    
    args.i = &i;
    args.argc = argc;
    args.argv = argv;
    args.champ = &champ;
    args.champ_count = 1;
    args.champs = champs;
    
    
    int result = parse_n_flag(&args);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(champ.number, 5);
    cr_assert_eq(*args.i, 2);

    
    i = 0;
    argc = 3;
    argv[1] = "abc";
    result = parse_n_flag(&args);
    
    cr_assert_eq(result, 84);
    
    
    i = 0;
    argv[1] = "-1";
    result = parse_n_flag(&args);
    
    cr_assert_eq(result, 84);
    
    
    i = 0;
    argv[1] = "3";
    result = parse_n_flag(&args);
    
    cr_assert_eq(result, 84);
}

Test(parser, test_parse_dump_flag)
{
    parse_n_flag_args_t args;
    vm_t vm;
    int i = 0;
    char *argv[] = {"-dump", "100", "filename.cor"};
    int argc = 3;
    
    
    initialize_vm(&vm);
    
    args.i = &i;
    args.argc = argc;
    args.argv = argv;
    args.champ = NULL;
    args.champ_count = 0;
    args.champs = NULL;
    
    
    int result = parse_dump_flag(&args, &vm);
    
    cr_assert_eq(result, 0);
    cr_assert(vm.options.dump_flag);
    cr_assert_eq(vm.options.dump_cycle, 100);
    cr_assert_eq(*args.i, 2);
    
    
    i = 0;
    argc = 3;
    argv[1] = "abc";
    result = parse_dump_flag(&args, &vm);
    
    cr_assert_eq(result, 84);
    
    
    i = 0;
    argv[1] = "-1";
    result = parse_dump_flag(&args, &vm);
    
    cr_assert_eq(result, 84);
    
    
    i = 0;
    argv[1] = "0";
    result = parse_dump_flag(&args, &vm);
    
    cr_assert_eq(result, 84);
}

Test(parser, test_parse_champion_flags)
{
    parse_champion_flags_args_t args;
    champion_t champ;
    vm_t vm;
    int i = 0;
    char *argv[] = {"-n", "5", "-a", "100", "filename.cor"};
    int argc = 5;
    
    
    initialize_vm(&vm);
    memset(&champ, 0, sizeof(champion_t));
    champ.number = -1;
    champ.load_address = -1;
    
    args.i = &i;
    args.argc = argc;
    args.argv = argv;
    args.champ = &champ;
    args.champ_count = 0;
    args.champs = NULL;
    
    
    int result = parse_champion_flags(args, &vm);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(champ.number, 5);
    cr_assert_eq(champ.load_address, 100);
    cr_assert_eq(*args.i, 4);
    
    
    i = 0;
    argv[0] = "-dump";
    argv[1] = "50";
    argv[2] = "-n";
    argv[3] = "3";
    champ.number = -1;
    
    result = parse_champion_flags(args, &vm);
    
    cr_assert_eq(result, 0);
    cr_assert(vm.options.dump_flag);
    cr_assert_eq(vm.options.dump_cycle, 50);
    cr_assert_eq(champ.number, 3);
    
    
    i = 0;
    argv[0] = "-unknown";
    
    result = parse_champion_flags(args, &vm);
    
    cr_assert_eq(result, 0); 
}


Test(memory_utils, test_get_param_value)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    get_param_value_args_t args;
    
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 100;
    process.registers[0] = 42; 
    
    
    args.process = &process;
    args.param_type = T_REG;
    args.param_value = 1; 
    args.memory = memory;
    args.is_modulo = true;
    
    int value = get_param_value(args);
    
    cr_assert_eq(value, 42);
    
    
    args.param_type = T_DIR;
    args.param_value = 123;
    
    value = get_param_value(args);
    
    cr_assert_eq(value, 123);
    
    
    write_memory(memory, 150, 456, 4);
    
    args.param_type = T_IND;
    args.param_value = 50; 
    
    value = get_param_value(args);
    
    cr_assert_eq(value, 456);
    
    
    args.param_type = 0;
    
    value = get_param_value(args);
    
    cr_assert_eq(value, 0);
}

Test(read_module, test_read_param_value)
{
    vm_t vm;
    process_t process;
    decode_context_t ctx;
    instruction_t instruction;
    int arg_value;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    memset(&instruction, 0, sizeof(instruction_t));
    
    
    process.pc = 0;
    vm.memory[2] = 3; 
    
    instruction.op_code = 2; 
    init_decode_context(&ctx, &vm, &process, &instruction);
    ctx.offset = 2;
    
    
    bool result = read_param_value(&ctx, T_REG, &arg_value);
    
    cr_assert(result);
    cr_assert_eq(arg_value, 3);
    cr_assert_eq(ctx.offset, 3);
    
    
    ctx.offset = 2;
    vm.memory[2] = 0; 
    
    result = read_param_value(&ctx, T_REG, &arg_value);
    
    cr_assert_not(result);
    cr_assert_eq(arg_value, 0);
    
    
    ctx.offset = 2;
    vm.memory[2] = 0x00;
    vm.memory[3] = 0x00;
    vm.memory[4] = 0x00;
    vm.memory[5] = 0x7B; 
    
    
    result = read_param_value(&ctx, T_DIR, &arg_value);
    
    cr_assert(result);
    cr_assert_eq(arg_value, 123);
    cr_assert_eq(ctx.offset, 6);
    
    
    ctx.offset = 2;
    vm.memory[2] = 0x00;
    vm.memory[3] = 0x0A; 
    
    
    result = read_param_value(&ctx, T_IND, &arg_value);
    
    cr_assert(result);
    cr_assert_eq(arg_value, 10);
    cr_assert_eq(ctx.offset, 4);
    
    
    ctx.offset = 2;
    
    result = read_param_value(&ctx, 0, &arg_value);
    
    cr_assert_not(result);
}


Test(instruction_execution, test_read_direct_param)
{
    vm_t vm;
    process_t process;
    decode_context_t ctx;
    instruction_t instruction;
    int arg_value;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    memset(&instruction, 0, sizeof(instruction_t));
    
    
    process.pc = 0;
    vm.memory[2] = 0x00;
    vm.memory[3] = 0x00;
    vm.memory[4] = 0x00;
    vm.memory[5] = 0x7B; 
    
    instruction.op_code = 2; 
    init_decode_context(&ctx, &vm, &process, &instruction);
    ctx.offset = 2;
    
    
    bool result = read_param_value(&ctx, T_DIR, &arg_value);
    
    cr_assert(result);
    cr_assert_eq(arg_value, 123);
    cr_assert_eq(ctx.offset, 6);
    
    
    process.pc = 10;
    vm.memory[12] = 0x00;
    vm.memory[13] = 0x7B; 
    
    instruction.op_code = 10; 
    init_decode_context(&ctx, &vm, &process, &instruction);
    ctx.offset = 2;
    
    
    result = read_param_value(&ctx, T_DIR, &arg_value);
    
    cr_assert(result);
    cr_assert_eq(arg_value, 123);
    cr_assert_eq(ctx.offset, 4);
}

Test(op_utils, test_set_get_register_value)
{
    process_t process;
    
    
    memset(&process, 0, sizeof(process_t));
    
    
    bool result = set_register_value(&process, 1, 42);
    
    cr_assert(result);
    cr_assert_eq(get_register_value(&process, 1), 42);
    
    
    result = set_register_value(&process, REG_NUMBER, 123);
    
    cr_assert(result);
    cr_assert_eq(get_register_value(&process, REG_NUMBER), 123);
    
    
    result = set_register_value(&process, 0, 456);
    
    cr_assert_not(result);
    cr_assert_eq(get_register_value(&process, 0), 0);
    
    
    result = set_register_value(&process, REG_NUMBER + 1, 789);
    
    cr_assert_not(result);
    cr_assert_eq(get_register_value(&process, REG_NUMBER + 1), 0);
}

Test(memory_utils, test_read_write_memory_wrap_around)
{
    byte_t memory[MEM_SIZE] = {0};
    
    
    write_memory(memory, MEM_SIZE - 2, 0x12345678, 4);
    
    
    cr_assert_eq(memory[MEM_SIZE - 2], 0x12);
    cr_assert_eq(memory[MEM_SIZE - 1], 0x34);
    cr_assert_eq(memory[0], 0x56);
    cr_assert_eq(memory[1], 0x78);
    
    
    int value = read_memory(memory, MEM_SIZE - 2, 4);
    
    cr_assert_eq(value, 0x12345678);
    
    
    write_memory(memory, -2, 0xABCD, 2);
    
    
    cr_assert_eq(memory[MEM_SIZE - 2], 0xAB);
    cr_assert_eq(memory[MEM_SIZE - 1], 0xCD);
    
    value = read_memory(memory, -2, 2);
    
    cr_assert_eq(value, 0xABCD);
}


Test(op_functions, test_op_ldi_detailed)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    
    memset(&process, 0, sizeof(process_t));
    process.pc = 0;
    
    
    write_memory(memory, 15, 123, 4);
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 5;  
    process.current_op_args[1] = 10; 
    process.current_op_args[2] = 1;  
    
    op_ldi(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[0], 123);
    cr_assert_not(process.carry);
    
    
    process.current_op_arg_types[0] = 0; 
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_REG;
    
    process.registers[0] = 0;
    
    op_ldi(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[0], 0); 
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_DIR;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 5;
    process.current_op_args[1] = 10;
    process.current_op_args[2] = 0; 
    
    op_ldi(&process, memory, NULL, 0);
    
}


Test(process_creation, test_create_process_with_allocation)
{
    vm_t vm;
    champion_t champ;
    
    
    initialize_vm(&vm);
    memset(&champ, 0, sizeof(champion_t));
    
    champ.number = 42;
    champ.load_address = 100;
    
    create_process(&vm, &champ);
    
    cr_assert_not_null(vm.processes);
    cr_assert_eq(vm.process_count, 1);
    cr_assert_eq(vm.processes->champion_number, 42);
    cr_assert_eq(vm.processes->pc, 100);
    cr_assert_eq(vm.processes->registers[0], 42);
    
    
    champ.number = 43;
    champ.load_address = 200;
    
    create_process(&vm, &champ);
    
    cr_assert_eq(vm.process_count, 2);
    cr_assert_eq(vm.processes->champion_number, 43);
    cr_assert_eq(vm.processes->pc, 200);
    cr_assert_eq(vm.processes->next->champion_number, 42);
    
    
    process_t *next = vm.processes->next;
    free(vm.processes);
    free(next);
}


Test(op_functions, test_op_and_detailed)
{
    process_t process;
    byte_t memory[MEM_SIZE] = {0};
    
    
    memset(&process, 0, sizeof(process_t));
    process.registers[0] = 0xF0F0; 
    process.registers[1] = 0x0F0F; 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 1; 
    process.current_op_args[1] = 2; 
    process.current_op_args[2] = 3; 
    
    op_and(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0x0000); 
    cr_assert(process.carry); 
    
    
    process.current_op_arg_types[0] = T_DIR;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 0xFF00; 
    process.current_op_args[1] = 2;      
    process.current_op_args[2] = 3;      
    
    op_and(&process, memory, NULL, 0);
    
    cr_assert_eq(process.registers[2], 0x0F00); 
    cr_assert_not(process.carry); 
    
    
    process.current_op_arg_types[0] = T_REG;
    process.current_op_arg_types[1] = T_REG;
    process.current_op_arg_types[2] = T_REG;
    process.current_op_args[0] = 1;
    process.current_op_args[1] = 2;
    process.current_op_args[2] = 0; 
    
    op_and(&process, memory, NULL, 0);
    
}


Test(read_module, test_read_coding_byte_details)
{
    vm_t vm;
    process_t process;
    decode_context_t ctx;
    instruction_t instruction;
    
    
    initialize_vm(&vm);
    memset(&process, 0, sizeof(process_t));
    memset(&instruction, 0, sizeof(instruction_t));
    
    
    process.pc = 0;
    vm.memory[1] = 0x90; 
    
    instruction.op_code = 2; 
    init_decode_context(&ctx, &vm, &process, &instruction);
    
    bool result = read_coding_byte(&ctx);
    
    cr_assert(result);
    cr_assert(ctx.has_coding_byte);
    cr_assert_eq(ctx.coding_byte, 0x90);
    cr_assert_eq(ctx.offset, 2);
    
    
    instruction.op_code = 1; 
    init_decode_context(&ctx, &vm, &process, &instruction);
    
    result = read_coding_byte(&ctx);
    
    cr_assert(result);
    cr_assert_not(ctx.has_coding_byte);
    cr_assert_eq(ctx.coding_byte, 0);
    cr_assert_eq(ctx.offset, 1);
}