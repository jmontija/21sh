/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_lib.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/02 15:43:58 by julio             #+#    #+#             */
/*   Updated: 2016/05/02 18:01:09 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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