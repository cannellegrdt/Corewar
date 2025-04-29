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

/* utilities */
int error_msg(char *str, int ret_value);
int print_help(void);

/* other functions */
void initialize_vm(vm_t *vm);
void free_vm(vm_t *vm);
void free_champions(champion_t **champions, int count);
int parse_arguments(int argc, char *argv[]);

#endif //COREWAR_H
