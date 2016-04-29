# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 17:28:13 by jmontija          #+#    #+#              #
#    Updated: 2016/04/30 00:07:44 by jmontija         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


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
SRCS			+=	srcs/exec_cmd.c
SRCS			+=	srcs/child.c
SRCS			+=	srcs/builtins.c
SRCS			+=	srcs/set_opt.c
SRCS			+=	srcs/env_opt.c

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
