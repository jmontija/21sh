/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/19 16:00:22 by julio             #+#    #+#             */
/*   Updated: 2016/06/22 22:55:46 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	exit_shell(t_group *grp, int exit_id)
{
	t_hist	*curr;
	int		hist;

	hist = open(
	"/nfs/2015/j/jmontija/.fsh_history", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (grp->curr_hist == NULL)
		grp->curr_hist = grp->hist;
	curr = grp->curr_hist;
	while (curr && curr->next != NULL)
		curr = curr->next;
	while (curr != NULL)
	{
		ft_putendl_fd(curr->name, hist);
		curr = curr->prev;
	}
	reset_shell();
	ft_putstr_fd("\n", 2);
	exit(exit_id);
}

void	set_grp_next2(t_group *grp)
{
	struct winsize	w;

	ioctl(0, TIOCGWINSZ, &w);
	grp->term = (t_term *)malloc(sizeof(t_term));
	grp->term->curs_pos = 0;
	grp->term->line = 0;
	grp->term->cmd_size = 0;
	grp->term->other_read = 0;
	grp->term->cmd_line = NULL;
	grp->term->search = NULL;
	grp->term->window = (t_window *)malloc(sizeof(t_window));
	grp->term->window->width = w.ws_col;
	grp->term->window->heigth = w.ws_row;
}

void	set_grp_next(t_group *grp)
{
	int	i;

	i = -1;
	grp->exit[0] = 0;
	grp->exit[1] = 0;
	grp->quotes = (int *)malloc(sizeof(int) * 6);
	while (++i < 6)
		grp->quotes[i] = 0;
	grp->define_cmd = (int *)malloc(sizeof(int) * 4);
	i = -1;
	while (++i < 4)
		grp->define_cmd[i] = false;
	grp->options = (t_options *)malloc(sizeof(t_options));
	grp->options->on = (t_bool *)malloc(sizeof(t_bool) * 7);
	grp->options->params = (char **)malloc(sizeof(char *) * 7);
	i = -1;
	while (++i < 7)
	{
		grp->options->on[i] = false;
		grp->options->params[i] = NULL;
	}
	set_grp_next2(grp);
}

t_group	*set_grp(void)
{
	t_group	*grp;
	int		i;

	i = -1;
	grp = (t_group *)malloc(sizeof(t_group));
	!(grp) ? exit(0) : 0;
	grp->first = NULL;
	grp->last = NULL;
	grp->cmd = NULL;
	grp->curr_cmd = NULL;
	grp->cmd_save = NULL;
	grp->env = NULL;
	grp->hist = NULL;
	grp->curr_hist = NULL;
	grp->tmp_from = NULL;
	grp->fd_in_save = STDIN_FILENO;
	grp->pipe = 0;
	grp->is_search = false;
	grp->prompt_size = 6;
	set_grp_next(grp);
	return (grp);
}

t_group	*init_grp(void)
{
	static t_group *grp = NULL;

	if (grp == NULL)
		grp = set_grp();
	return (grp);
}
