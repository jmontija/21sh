/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_lib.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/02 15:43:58 by julio             #+#    #+#             */
/*   Updated: 2016/06/20 23:28:12 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		ft_go_home(t_group *grp)
{
	while (TERM(curs_pos) > 0)
		ft_left_arrow(grp);
}

void		ft_go_end(t_group *grp)
{
	while (TERM(curs_pos) < TERM(cmd_size))
		ft_right_arrow(grp);
}

void		ft_go_up(t_group *grp)
{
	int i;

	i = -1;
	while (++i < TERM(window->width) && TERM(curs_pos) > 0)
		ft_left_arrow(grp);
}

void		ft_go_down(t_group *grp)
{
	int i;

	i = -1;
	while (++i < TERM(window->width) && TERM(curs_pos) < TERM(cmd_size))
		ft_right_arrow(grp);
}

void		insert_hist(t_group *grp, char *name)
{
	t_hist	*new;

	grp->curr_hist = NULL;
	if (ft_strcmp("", name) == 0 ||
		(grp->hist && ft_strcmp(grp->hist->name, name) == 0))
		return ;
	new = (t_hist *)malloc(sizeof(t_hist));
	new->name = SDUP(name);
	new->next = grp->hist;
	new->prev = NULL;
	if (grp->hist != NULL)
		grp->hist->prev = new;
	grp->hist = new;
}
