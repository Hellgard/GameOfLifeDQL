##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## compiles n stuff
##

PATH_S		=	./my_server/
PATH_C		=	./my_client/

MAKE_S		=	-C $(PATH_S)
MAKE_C		=	-C $(PATH_C)

NAME_S		=	myftp
NAME_C		=	myftp_client

BLUE	=	"\e[1;34m"
ECHO	=	/bin/echo -e
BOLD_T	=	"\e[1m"
LINE_RETURN	=	$(ECHO) ""
COLOR_THEME	=	$(BLUE)

all:	message $(NAME_S) $(NAME_C)

message:
		@$(LINE_RETURN)
		@$(ECHO) $(BOLD_T)$(COLOR_THEME)"  ____                      _ _                "$(DEFAULT)
		@$(ECHO) $(BOLD_T)$(COLOR_THEME)" / ___|___  _ __ ___  _ __ (_) |(_) _ __   ___ "$(DEFAULT)
		@$(ECHO) $(BOLD_T)$(COLOR_THEME)"| |   / _ \|  _\  _\ |  _ \| | || ||  _ \ / _  \""$(DEFAULT)
		@$(ECHO) $(BOLD_T)$(COLOR_THEME)"| |__| (_) | | | | | | |_) | | || || | | | (_) |"$(DEFAULT)
		@$(ECHO) $(BOLD_T)$(COLOR_THEME)" \____\___/|_| |_| |_| .__/|_|_||_||_| |_|\__  |"$(DEFAULT)
		@$(ECHO) $(BOLD_T)$(COLOR_THEME)"                     |_|                    _| |"$(DEFAULT)
		@$(ECHO) $(BOLD_T)$(COLOR_THEME)"                     |_|                   |___|"$(DEFAULT)
		@$(LINE_RETURN)

$(NAME):
	printf "\033[0;32m"
	@echo && $(MAKE) $(MAKE_S)
	cp $(PATH_S)/$(NAME_S) .
	@echo && $(MAKE) $(MAKE_C)
	cp $(PATH_C)/$(NAME_C) .

$(NAME_S):
	@echo && $(MAKE) $(MAKE_S)
	cp $(PATH_S)/$(NAME_S) .

$(NAME_C):
	@echo && $(MAKE) $(MAKE_C)
	cp $(PATH_C)/$(NAME_C) .

clean:
	@echo && $(MAKE) clean $(MAKE_S)
	rm -rf $(NAME_S)
	@echo && $(MAKE) clean $(MAKE_C)
	rm -rf $(NAME_C)

fclean:
	@echo && $(MAKE) fclean $(MAKE_S)
	rm -rf $(NAME_S)
	@echo && $(MAKE) fclean $(MAKE_C)
	rm -rf $(NAME_C)

re: fclean all

.PHONY:		all			\
			clean		\
			fclean		\
			re
