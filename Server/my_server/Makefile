##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## compiles n stuff
##

CC 			=	gcc

NAME		=	myftp

NAME_EXEC	=	./$(NAME)

SRC			=	sources/

SRC_C		=	$(wildcard $(SRC)*.c)	\
				$(wildcard $(SRC)*/*.c)	\
				$(wildcard $(SRC)*/*/*.c)
OBJ_C       =   $(SRC_C:%.c=%.o)

OBJS 		=	$(OBJ_C)

COMPILER	=	gcc -o
RM			=	rm -rf
TRASH		=	vgcore* *~ *.html

CFLAGS		=	-I./includes/
CFLAGS		+=	-g3 -O1 -pedantic
CFLAGS		+=	-W -Wall -Wextra -Wpedantic -Wno-long-long
VFLAGS		=	-s --leak-check=full --show-leak-kinds=all --track-origins=yes

PRINT_D		=	@echo "\e[92m\e[1mDONE\e[0m"
PRINT_LN	=	yes - | head -n $$(tput cols) | tr -d '\n'
PRINT_SP	=	$(PRINT_D); $(PRINT_LN)

all:		$(NAME)

print_b:
			@echo "\e[93m\e[1mBuilding $(NAME)\n\e[0m"

$(NAME):	print_b $(OBJS)
			$(COMPILER) $(NAME) $(CFLAGS) $(OBJS)
			$(PRINT_SP)
			@echo "\e[95m\e[1mSuccessfully built $(NAME)\e[0m"
			@$(PRINT_LN)

clean:
			@$(PRINT_LN)
			@echo "\e[94m\e[1mRemoving object files\n\e[0m"
			$(RM) $(OBJS) $(TRASH)
			$(PRINT_SP)
			@echo "\e[95m\e[1mSuccessfully cleaned\e[0m"
			@$(PRINT_LN)

fclean:		clean
			@echo "\e[94m\e[1mRemoving $(NAME)\n\e[0m"
			$(RM) $(NAME)
			$(PRINT_SP)
			@echo "\e[95m\e[1mSuccessfully fcleaned\e[0m"
			@$(PRINT_LN)

re:			fclean all

tests_mem:	re
			@echo "\e[93m\e[1mRunning $(NAME) with valgrind\e[0m"
			@echo && valgrind $(NAME_EXEC)
			$(PRINT_SP)
			@echo "\e[95m\e[1mSuccessfully ran $(NAME) valgrind\e[0m"
			@$(PRINT_LN)

tests_fmem:	re
			@echo "\e[93m\e[1mRunning $(NAME) with full valgrind\e[0m"
			@echo && valgrind $(VFLAGS) $(NAME_EXEC)
			$(PRINT_SP)
			@echo "\e[95m\e[1mSuccessfully ran $(NAME) full valgrind\e[0m"
			@$(PRINT_LN)

%.o:		%.c
			@echo "$< ---> $@"
			@$(CC) $(CFLAGS) -c $< -o $@

.PHONY:		all			\
			make_lib	\
			clean		\
			fclean		\
			re			\
			tests_mem	\
			tests_fmem
