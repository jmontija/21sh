/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_search_lib.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/28 17:32:39 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/21 19:04:41 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	up_clear_find(t_group *grp, char *name)
{
	int	i;

	i = -1;
	tputs(tgetstr("cr", NULL), 0, ft_getchar);
	while (++i < TERM(line))
		tputs(tgetstr("up", NULL), 0, ft_getchar);
	tputs(tgetstr("cd", NULL), 0, ft_getchar);
	show_prompt(grp, "fsh-> ", 6, "\033[1;32m");
	TERM(line) = 0;
}

void	display_search(t_group *grp, char *name)
{
	int	i;

	i = -1;
	up_clear_find(grp, name);
	ft_putendl_fd(name, 2);
	while (++i < LEN(name))
		if ((START_POS + i) % TERM(window->width) == 0)
			TERM(line) += 1;
	TERM(line) += 1;
	show_prompt(grp, "fsh: search: ", 13, "\033[1;34m");
	ft_putstr_fd(TERM(cmd_line), 2);
}

void	find_search(t_group *grp)
{
	t_hist *curr;

	if (TERM(curs_pos) == 0)
	{
		display_search(grp, TERM(cmd_save));
		return ;
	}
	curr = grp->curr_hist == NULL ? grp->hist : grp->curr_hist;
	while (curr != NULL)
	{
		if (ft_strstr(curr->name, TERM(cmd_line)) != NULL)
		{
			display_search(grp, curr->name);
			break ;
		}
		curr = curr->next;
	}
}

void	canceled_search(t_group *grp)
{
	int	i;

	i = -1;
	grp->is_search = false;
	up_clear_find(grp, TERM(cmd_save));
	ft_putstr_fd(TERM(cmd_save), 2);
	TERM(cmd_line) = SDUP(TERM(cmd_save));
	TERM(curs_pos) = LEN(TERM(cmd_line));
	TERM(cmd_size) = TERM(curs_pos);
}

void	init_search(t_group *grp)
{
	int	i;

	i = -1;
	TERM(line) = 0;
	if (grp->is_search == false)
	{
		grp->is_search = true;
		TERM(cmd_save) = SDUP(TERM(cmd_line));
		while (++i < TERM(curs_pos))
			if ((START_POS + i) % TERM(window->width) == 0)
				TERM(line) += 1;
		TERM(line) += 1;
		TERM(cmd_size) = 0;
		TERM(curs_pos) = 0;
		ft_bzero(TERM(cmd_line), LEN(TERM(cmd_line)));
		tputs(tgetstr("do", NULL), 0, ft_getchar);
		show_prompt(grp, "fsh: search: ", 13, "\033[1;34m");
	}
	else
		canceled_search(grp);
}
