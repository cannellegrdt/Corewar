##
## EPITECH PROJECT, 2024
## B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
## File description:
## Makefile
##

SRC	=	src/check_lives.c 	\
		src/create_champion.c	\
		src/creates.c	\
		src/decode_args.c	\
		src/decode_args_with_coding_byte.c	\
		src/dump_memory.c	\
		src/init_and_free.c	\
		src/instructions.c	\
		src/load_champion.c	\
		src/op.c	\
		src/read.c	\
		src/run_vm.c	\
		src/instructions/op_add.c	\
		src/instructions/op_aff.c	\
		src/instructions/op_and.c	\
		src/instructions/op_fork.c	\
		src/instructions/op_ld.c	\
		src/instructions/op_ldi.c	\
		src/instructions/op_lfork.c	\
		src/instructions/op_live.c	\
		src/instructions/op_lld.c	\
		src/instructions/op_lldi.c	\
		src/instructions/op_or.c	\
		src/instructions/op_st.c	\
		src/instructions/op_sti.c	\
		src/instructions/op_sub.c	\
		src/instructions/op_xor.c	\
		src/instructions/op_zjmp.c	\
		src/utilities/error_msg.c	\
		src/utilities/load_address.c	\
		src/utilities/op_utilities.c	\
		src/utilities/print_help.c	\
		src/utilities/read_short_int.c	\
		src/utilities/sort_process.c	\
		src/utilities/swap_endian.c 	\
		src/parser/parse_arguments.c	\
		src/parser/parse_flags.c


LIB_SRC	=	lib/my/my_getnbr.c	\
		lib/my/my_memset.c	\
		lib/my/my_putnbr.c	\
		lib/my/my_strcmp.c	\
		lib/my/my_strdup.c	\
		lib/my/my_strlen.c	\
		lib/my/my_strncpy.c	\
		lib/my/endswith.c

MAIN_SRC	= main.c

OBJ	=	$(SRC:.c=.o) $(MAIN_SRC:.c=.o)
LIB_OBJ	=	$(LIB_SRC:.c=.o)

NAME	=	corewar
LIB	=	libmy.a

LDFLAGS    = -L. -lmy
CFLAGS	+=	-Wall -Wextra -Wpedantic -Werror
CPPFLAGS	+=	-Iinclude/

all: $(LIB) $(NAME)

$(NAME): $(OBJ) $(LIB)
	gcc -o $(NAME) $(OBJ) -L. -lmy -g3

$(LIB): $(LIB_OBJ)
	ar rc $(LIB) $(LIB_OBJ)
	ranlib $(LIB)

clean:
	$(RM) $(OBJ) $(LIB_OBJ)

fclean: clean
	$(RM) $(NAME) $(LIB)
	$(RM) unit_tests *.gcno *.gcda

re: fclean all

coding_style: fclean
	coding-style . > /dev/null
	cat coding-style-reports.log
	$(RM) coding-style-reports.log

unit_tests: fclean all
	gcc -o unit_tests tests/test_corewar.c $(SRC) -Iinclude/ \
	--coverage -lcriterion -lreadline
	./unit_tests

gcovr: unit_tests
	gcovr --exclude tests --gcov-ignore-parse-errors=negative_hits.warn
	$(RM) unit_tests *.gcno *.gcda

func_tests: fclean all
	cd tester && sh tester.sh && cd ..

tests: unit_tests func_tests

.PHONY: all clean fclean re coding_style unit_tests gcovr func_tests tests
