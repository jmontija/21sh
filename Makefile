# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 17:28:13 by jmontija          #+#    #+#              #
#    Updated: 2016/06/21 01:35:16 by jmontija         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -fsanitize=address (force segflt)
NAME			=	21sh
COMPIL			=	gcc

LIB_PATH		=	libft/
LIB				=	$(LIB_PATH)libft.a
LIB_LINK		=	-L $(LIB_PATH) -lft

INCLUDES		=	-I $(LIB_PATH)/includes -I ./includes
SRCS			+=	srcs/main.c
SRCS			+=	srcs/shell_lib.c
SRCS			+=	srcs/lst_lib.c
SRCS			+=	srcs/msh_lib.c
SRCS			+=	srcs/father.c
SRCS			+=	srcs/keyboard.c
SRCS			+=	srcs/keyboard_tools.c
SRCS			+=	srcs/keyboard_tools2.c
SRCS			+=	srcs/keyboard_tools3.c
SRCS			+=	srcs/keyboard_lib.c
SRCS			+=	srcs/keyboard_search_lib.c
SRCS			+=	srcs/child.c
SRCS			+=	srcs/builtins.c
SRCS			+=	srcs/env.c
SRCS			+=	srcs/env_opt.c
SRCS			+=	srcs/env_lib.c
SRCS			+=	srcs/env_lib2.c
SRCS			+=	srcs/pre_exec.c
SRCS			+=	srcs/redir_exec.c
SRCS			+=	srcs/exec.c
SRCS			+=	srcs/parser.c
SRCS			+=	srcs/remake_cmd.c
SRCS			+=	srcs/remake_lib_cmd.c
SRCS			+=	srcs/remake_lib_file.c
SRCS			+=	srcs/signaux.c

OBJS			=	$(SRCS:srcs/%.c=srco/%.o)

all: srco $(NAME)

srco:
	@mkdir -p srco

$(NAME): $(LIB) $(OBJS)
	@$(COMPIL) -o $@ $^ $(LIB_LINK) -ltermcap
	@echo "Compiled" $(NAME) "done"

$(LIB):
	@make -C $(LIB_PATH)
	@echo "Created libft.a"

srco/%.o: srcs/%.c ./includes/shell.h
	@$(COMPIL) $(INCLUDES) -c -o $@ $<

clean:
	@rm -f $(OBJS)
	@rm -rf srco
	@make clean -C $(LIB_PATH)
	@echo "Cleaning" $(NAME)

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIB_PATH) fclean
	@echo "Delete" $(NAME)

re: fclean all
