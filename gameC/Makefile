##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## compiles n stuff
##

CC 			=	gcc

NAME		=	zappy

MAIN		=	main.c
SRC_C 		=	source/Game.c \
				source/Player.c \
				source/Map.c \
				source/Inventory.c

OBJ_M		=	$(MAIN:%.c=%.o)
OBJ_C       =   $(SRC_C:%.c=./%.o)
OBJS 		=	$(OBJ_M) $(OBJ_C)

COMPILER	=	gcc -o
RM			=	rm
TRASH		=	vgcore* *~ *.html

CFLAGS		= 	-I include/
CFLAGS		+=	-g3 -O1 -pedantic
CFLAGS		+=	-W -Wall -Wextra -Wpedantic -Wno-long-long #-Werror
VFLAGS		=	-s --leak-check=full --show-leak-kinds=all --track-origins=yes

PRINT_D		=	@echo "\e[92m\e[1mDONE\e[0m"
PRINT_LN	=	yes - | head -n $$(tput cols) | tr -d '\n'
PRINT_SP	=	$(PRINT_D); $(PRINT_LN)

all:		$(NAME)

print_b:
			@echo "\e[93m\e[1mBuilding $(NAME)\n\e[0m"

$(NAME): $(OBJS)
		$(COMPILER) $(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
		rm main.o Game.o Player.o Map.o Inventory.o

fclean: clean
		$(RM) $(NAME).exe

tests_mem:	re
			@echo "\e[93m\e[1mRunning $(NAME) with valgrind\e[0m"
			@echo && valgrind ./$(NAME) -g
			$(PRINT_SP)
			@echo "\e[95m\e[1mSuccessfully ran $(NAME) valgrind\e[0m"
			@$(PRINT_LN)

tests_fmem:	re
			@echo "\e[93m\e[1mRunning $(NAME) with full valgrind\e[0m"
			@echo && valgrind $(VFLAGS) ./$(NAME) -g
			$(PRINT_SP)
			@echo "\e[95m\e[1mSuccessfully ran $(NAME) full valgrind\e[0m"
			@$(PRINT_LN)

re:			fclean all

.PHONY:		all			\
			clean		\
			fclean		\
			re