/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_tools2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/10 17:06:09 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/14 00:47:46 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	handling_historic(t_group *grp, int key)
{
	remove_line(grp);
	if (key == ARROW_U && grp->hist)
	{
		if (grp->curr_hist == NULL)
			grp->curr_hist = grp->hist;
		else if (grp->curr_hist->next)
			grp->curr_hist = grp->curr_hist->next;
		ft_putstr_fd(grp->curr_hist->name, 2);
		TERM(cmd_line) = SDUP(grp->curr_hist->name);
		TERM(curs_pos) += LEN(TERM(cmd_line));
		TERM(cmd_size) = TERM(curs_pos);
	}
	else if (key == ARROW_D && grp->hist)
	{
		if (grp->curr_hist != NULL)
			grp->curr_hist = grp->curr_hist->prev;
		if (grp->curr_hist != NULL)
		{
			ft_putstr_fd(grp->curr_hist->name, 2);
			TERM(cmd_line) = SDUP(grp->curr_hist->name);
			TERM(curs_pos) += LEN(TERM(cmd_line));
			TERM(cmd_size) = TERM(curs_pos);
		}
		else
			remove_line(grp);
	}
}

void		ft_left_arrow(t_group *grp)
{
	int i;

	if (((TERM(curs_pos) + START_POS) % TERM(window->width)) == 0)
	{
		i = 0;
		tputs(tgetstr("up", NULL), 0, ft_getchar);
		while (++i < TERM(window->width))
			tputs(tgetstr("nd", NULL), 0, ft_getchar);
	}
	else
		tputs(tgetstr("le", NULL), 0, ft_getchar);
	TERM(curs_pos) -= 1;
}

void		ft_right_arrow(t_group *grp)
{
	TERM(curs_pos) += 1;
	if (((TERM(curs_pos) + START_POS) % TERM(window->width)) == 0)
		tputs(tgetstr("do", NULL), 0, ft_getchar);
	else
		tputs(tgetstr("nd", NULL), 0, ft_getchar);
}

void	handling_arrow(t_group *grp, int key)
{
	if (key == ARROW_U || key == ARROW_D)
		handling_historic(grp, key);
	else if (key == ARROW_L && TERM(curs_pos) > 0)
		ft_left_arrow(grp);
	else if (key == ARROW_R && TERM(curs_pos) < LEN(TERM(cmd_line)))
		ft_right_arrow(grp);
}
