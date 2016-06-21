/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/18 18:39:25 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/21 18:56:54 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	handler_win(int sig)
{
	t_group			*grp;
	struct winsize	w;

	grp = init_grp();
	ioctl(0, TIOCGWINSZ, &w);
	TERM(window->width) = w.ws_col;
	TERM(window->heigth) = w.ws_row;
}

void	handler_ctrl_c(int sig)
{
	t_group	*grp;

	grp = init_grp();
	if (TERM(cmd_line) == NULL)
	{
		ft_putchar_fd('\n', 2);
		grp->exit[1] = true;
		return ;
	}
	ft_go_end(grp);
	ft_bzero(TERM(cmd_line), LEN(TERM(cmd_line)));
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	if (grp->is_search)
		canceled_search(grp);
	else if (TERM(other_read) == true)
	{
		TERM(other_read) = false;
		ioctl(0, TIOCSTI, "\n");
		grp->exit[1] = true;
	}
	else
		show_prompt(grp, "\nfsh-> ", 6, "\033[1;32m");
}

void	ft_prompt(int signum)
{
	char	order[2];
	t_group	*grp;

	signum = 0;
	order[0] = 3;
	order[1] = 0;
	grp = init_grp();
	show_prompt(grp, "\nfsh-> ", 6, "\033[1;32m");
	ioctl(0, TIOCSTI, order);
}

void	sig_handler(void)
{
	signal(SIGTSTP, ft_prompt);
	signal(SIGQUIT, handler_ctrl_c);
	signal(SIGINT, handler_ctrl_c);
	signal(SIGWINCH, handler_win);
}
