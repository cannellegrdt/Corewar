##
## EPITECH PROJECT, 2024
## B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
## File description:
## Makefile
##

SRC	=	src/null_src.c

MAIN_SRC	= main.c

OBJ	=	$(SRC:.c=.o) $(MAIN_SRC:.c=.o)

NAME	=	corewar

CFLAGS	+=	-Wall -Wextra -Wpedantic -Werror
CPPFLAGS	+=	-Iinclude/

ifeq ($(ENV), dev)
	CFLAGS	+=	-g3
endif

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	$(RM) unit_tests *.gcno *.gcda

re: fclean all

coding_style: fclean
	coding-style . > /dev/null
	cat coding-style-reports.log
	$(RM) coding-style-reports.log

unit_tests: fclean all
	gcc -o unit_tests tests/test_mysh.c $(SRC) -Iinclude/ \
	--coverage -lcriterion -lreadline
	./unit_tests

gcovr: unit_tests
	gcovr --exclude tests --gcov-ignore-parse-errors=negative_hits.warn
	$(RM) unit_tests *.gcno *.gcda

func_tests: fclean all
	cd tester && sh tester.sh && cd ..

tests: unit_tests func_tests

.PHONY: all clean fclean re coding_style unit_tests gcovr func_tests tests
