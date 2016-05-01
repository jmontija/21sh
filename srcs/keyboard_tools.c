/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:31:01 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/01 19:50:15 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	arrow_ud(t_group *grp)
{
	ft_putstr("historique ");
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
		arrow_ud(grp);
	else if (key == ARROW_L && grp->curs_col > START_POS)
	{
		grp->curs_col -= 1;
		ft_tputs("le", NULL);
	}
	else if (key == ARROW_R && grp->curs_col < LEN(*cmd) + START_POS)
	{
		grp->curs_col += 1;
		ft_tputs("nd", NULL);
	}
}