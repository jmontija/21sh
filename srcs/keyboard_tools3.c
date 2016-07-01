/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_tools3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/13 20:08:18 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/22 19:41:03 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_prev_word(t_group *grp)
{
	while (TERM(curs_pos) > 0 && TERM(cmd_line)[TERM(curs_pos) - 1] == ' ')
		ft_left_arrow(grp);
	while (TERM(curs_pos) > 0)
	{
		if (TERM(curs_pos) > 0 && TERM(cmd_line)[TERM(curs_pos) - 1] == ' ')
			break ;
		ft_left_arrow(grp);
	}
}

void	ft_next_word(t_group *grp)
{
	int bol;

	bol = 0;
	while (TERM(curs_pos) < TERM(cmd_size) &&
	(TERM(cmd_line)[TERM(curs_pos)] == ' ' || bol == 0))
	{
		if (TERM(cmd_line)[TERM(curs_pos)] == ' ')
			bol = 1;
		ft_right_arrow(grp);
	}
}

void	handling_clear_screen(t_group *grp)
{
	tputs(tgetstr("cl", NULL), 0, ft_getchar);
	show_prompt(grp, "fsh-> ", 6, "\033[1;32m");
	ft_putstr_fd(TERM(cmd_line), 2);
	TERM(curs_pos) = LEN(TERM(cmd_line));
}

void	remove_line(t_group *grp)
{
	ft_go_end(grp);
	while (TERM(curs_pos) > 0)
		handling_backspace(grp);
}

void	reset_edl(t_group *grp)
{
	if (grp->is_search == true)
	{
		TERM(cmd_line) = SDUP(TERM(search));
		TERM(cmd_line) == NULL ? (TERM(cmd_line) = ft_strnew(0)) : 0;
		grp->is_search = false;
		REMOVE(&TERM(search));
		REMOVE(&TERM(cmd_save));
	}
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
}
