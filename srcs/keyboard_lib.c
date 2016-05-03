/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_lib.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/02 15:43:58 by julio             #+#    #+#             */
/*   Updated: 2016/05/02 23:25:10 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	get_cursor_pos()
{
	struct winsize	window;

	ioctl(0, TIOCGWINSZ, &window);
	
}

void	remove_line(t_group *grp, char **cmd)
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

void	insert_hist(t_group *grp, char *name)
{
	t_hist	*new;

	grp->curr_hist = NULL;
	if (ft_strcmp("", name) == 0 ||
		(grp->hist && ft_strcmp(grp->hist->name, name) == 0))
		return ;
	new = (t_hist *)malloc(sizeof(t_hist));
	new->name = name;
	new->next = grp->hist;
	new->prev = NULL;
	if (grp->hist != NULL)
		grp->hist->prev = new;
	grp->hist = new;
}