/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** corewar.h
*/

#ifndef COREWAR_H
    #define COREWAR_H
    #include "op.h"
    #include "my.h"
    #include <stdbool.h>
    #include <unistd.h>
    #include <stdlib.h>

typedef unsigned char byte_t;

typedef struct champion_s {
    char *filename;
    int number;
    int load_address;
    int size;
    char name[PROG_NAME_LENGTH + 1];
    char comment[COMMENT_LENGTH + 1];
    byte_t *code;
    bool alive;
    int last_live;
    int lives_in_period;
} champion_t;

typedef struct process_s {
    int champion_number;
    int pc;
    int registers[REG_NUMBER];
    bool carry;
    int wait_cycles;
    int current_op;
    int last_live_cycle;
    bool alive;
    int current_op_args[MAX_ARGS_NUMBER];
    int current_op_arg_types[MAX_ARGS_NUMBER];
    struct vm_s *vm;
    struct process_s *next;
} process_t;

typedef struct instruction_s {
    int op_code;
    int arg_type[MAX_ARGS_NUMBER];
    int args[MAX_ARGS_NUMBER];
    int size;
} instruction_t;

typedef struct options_s {
    bool dump_flag;
    int dump_cycle;
} options_t;

typedef void (*op_function_t)(process_t *, byte_t *, champion_t **, int);

typedef struct vm_s {
    byte_t memory[MEM_SIZE];
    champion_t **champions;
    int champion_count;
    process_t *processes;
    int process_count;
    int cycle_counter;
    int cycle_to_die;
    int checks_performed;
    int lives_in_period;
    int last_alive_champion;
    options_t options;
    op_function_t op_functions[16];
} vm_t;

typedef struct decode_context_s {
    vm_t *vm;
    int pc;
    int offset;
    byte_t opcode;
    bool has_coding_byte;
    byte_t coding_byte;
    instruction_t *inst;
} decode_context_t;

/// CODING STYLE ///
typedef struct {
    int *i;
    int argc;
    char **argv;
    champion_t *champ;
    int champ_count;
    champion_t **champs;
} parse_n_flag_args_t;

typedef struct {
    int *i;
    int argc;
    char **argv;
    champion_t *champ;
    int champ_count;
    champion_t **champs;
} parse_champion_flags_args_t;
/// CODING STYLE ///

/* utilities */
int error_msg(char *str, int ret_value);
int print_help(void);
int swap_endian(int value);
void sort_processes_by_champion_number(vm_t *vm);
int calculate_address(int base_address, int offset, bool apply_idx_mod);
void dump_memory(byte_t *memory);
int read_short(byte_t *memory, int pos);
int read_int(byte_t *memory, int pos);
int get_register_value(process_t *process, int reg_num);
bool set_register_value(process_t *process, int reg_num, int value);
int read_memory(byte_t *memory, int address, int size);
void write_memory(byte_t *memory, int address, int value, int size);
int get_param_value(process_t *process, int param_type, int param_value,
    byte_t *memory, bool is_modulo);

/* other functions */
void initialize_vm(vm_t *vm);
void free_vm(vm_t *vm);
void free_champions(champion_t **champions, int count);
int parse_arguments(int argc, char *argv[]);
int read_champion_file(champion_t *champ);
void create_process(vm_t *vm, champion_t *champ);
int find_available_number(int next_number, int champ_count,
    champion_t **champs);
champion_t *init_champion(void);
int load_champions(vm_t *vm);
void run_vm(vm_t *vm);
int decode_args(vm_t *vm, process_t *process);
void execute_instruction(vm_t *vm, process_t *process);
void init_decode_context(decode_context_t *ctx, vm_t *vm,
    process_t *process, instruction_t *instruction);
int get_param_type(decode_context_t *ctx, int param_index);
bool read_param_value(decode_context_t *ctx, int arg_type,
    int *arg_value);
bool read_coding_byte(decode_context_t *ctx);

/* instructions */
void op_live(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_ld(process_t *process, byte_t *memory, champion_t **champions
    __attribute__((unused)), int champion_count __attribute__((unused)));
void op_st(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_add(process_t *process, byte_t *memory, champion_t **champions
    __attribute__((unused)), int champion_count __attribute__((unused)));
void op_sub(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_and(process_t *process, byte_t *memory, champion_t **champions
    __attribute__((unused)), int champion_count __attribute__((unused)));
void op_or(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_xor(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_zjmp(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_ldi(process_t *process, byte_t *memory, champion_t **champions
    __attribute__((unused)), int champion_count __attribute__((unused)));
void op_sti(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_fork(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_lld(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_lldi(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_lfork(process_t *process, byte_t *memory, champion_t **champions,
    int champion_count);
void op_aff(process_t *process, byte_t *memory __attribute__((unused)),
    champion_t **champions __attribute__((unused)), int champion_count
    __attribute__((unused)));

#endif //COREWAR_H
