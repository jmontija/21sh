/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:31:01 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/02 15:39:04 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	remove_line(t_group *grp, char **cmd)
{
	size_t	i;

	i = -1;
	grp->curs_col = START_POS + LEN(*cmd) - 1;
	ft_tputs(NULL, "ch");
	while (++i < LEN(*cmd))
	{
		ft_tputs("le", NULL);
		ft_tputs("dc", NULL);
	}
	REMOVE(cmd);
	*cmd = SDUP("");
	grp->curs_col = START_POS;
}

void	ft_historique(t_group *grp, char **cmd, int key)
{
	if (grp->hist == NULL)
		return ;
	remove_line(grp, cmd);
	if (key == ARROW_U)
	{
		if (grp->curr_hist == NULL)
			grp->curr_hist = grp->hist;
		else if (grp->curr_hist->next)
			grp->curr_hist = grp->curr_hist->next;
		ft_putstr_fd(grp->curr_hist->name, 2);
		*cmd = SDUP(grp->curr_hist->name);
		grp->curs_col += LEN(*cmd);
	}
	else if (key == ARROW_D)
	{
		if (grp->curr_hist != NULL)
			grp->curr_hist = grp->curr_hist->prev;
		if (grp->curr_hist != NULL)
		{
			ft_putstr_fd(grp->curr_hist->name, 2);
			*cmd = SDUP(grp->curr_hist->name);
			grp->curs_col += LEN(*cmd);
		}
		else
			remove_line(grp, cmd);
	}
}

void	handling_backspace(t_group *grp, char **cmd)
{
	char	*beg_cmd;
	char	*end_cmd;

	if (grp->curs_col <= START_POS)
		return ;
	beg_cmd = SUB(*cmd, 0, grp->curs_col - START_POS - 1);
	end_cmd = SUB(*cmd, grp->curs_col - START_POS, LEN(*cmd));
	*cmd = JOIN(beg_cmd, end_cmd);
	ft_tputs("le", NULL);
	ft_tputs("dc", NULL);
	grp->curs_col -= 1;
	REMOVE(&beg_cmd);
	REMOVE(&end_cmd);
}

void	handling_arrow(t_group *grp, char **cmd, int key)
{
	if (key == ARROW_U || key == ARROW_D)
		ft_historique(grp, cmd, key);
	else if (key == ARROW_L && grp->curs_col > START_POS)
	{
		grp->curs_col -= 1;
		tgetflag("bw");
		ft_tputs("le", NULL);
	}
	else if (key == ARROW_R && grp->curs_col < LEN(*cmd) + START_POS)
	{
		grp->curs_col += 1;
		ft_tputs("nd", NULL);
	}
}