/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:31:01 by jmontija          #+#    #+#             */
/*   Updated: 2016/04/29 23:31:14 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int		ft_getchar(int c)
{
	return (write(2, &c, 1));
}

void	arrow_ud(t_group *grp)
{
	ft_putstr("historique ");
}

void	arrow_lr(int lr, t_group *grp)
{

}

void	handling_arrow(t_group *grp, char **cmd, int key)
{
	if (key == ARROW_U || key == ARROW_D)
		arrow_ud(grp);
	else if (key == ARROW_L && grp->curs_pos > START_POS)
	{
		grp->curs_pos -= 1;
		ft_tputs("le", NULL);
	}
	else if (key == ARROW_R && grp->curs_pos < LEN(*cmd) + START_POS)
	{
		grp->curs_pos += 1;
		ft_tputs("nd", NULL);
	}
}