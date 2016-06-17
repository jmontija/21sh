/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/19 16:00:22 by julio             #+#    #+#             */
/*   Updated: 2016/06/17 03:01:30 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		unset_env(t_group *grp, char *todel)
{
	t_env *curr;
	t_env *prev;

	prev = NULL;
	curr = grp->first;
	ft_strcmp(todel, "_") == 0 ? todel = NULL : 0;
	while (todel != NULL && curr != NULL)
	{
		if ((ft_strcmp(curr->name, todel) == 0))
		{
			if (prev == NULL)
				grp->first = curr->next;
			else
				prev->next = curr->next;
			curr->next == NULL ? grp->last = prev : 0;
			REMOVE(&curr->name);
			REMOVE(&curr->val);
			ft_memdel((void *)&curr);
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (-1);
}

int		is_env(char *env)
{
	int i;

	i = -1;
	while (env[++i] != '\0')
	{
		if (env[i] == '=')
			break ;
	}
	if (env[i] != '=')
		return (false);
	return (i);
}

t_env	*create_env_line(t_group *grp, char *env, int i)
{
	t_env	*new;
	t_env	*curr_env;

	curr_env = grp->first;
	new = (t_env *)malloc(sizeof(t_env));
	if (!(new) || !(env))
		return (NULL);
	if ((i = is_env(env)) == false)
		return (NULL);
	new->exist = false;
	new->name = SUB(env, 0, i);
	new->val = SUB(env, i + 1, LEN(env));
	while (curr_env != NULL)
	{
		if (ft_strcmp(new->name, curr_env->name) == 0)
		{
			new->exist = true;
			curr_env->val = new->val;
			break ;
		}
		curr_env = curr_env->next;
	}
	new->next = NULL;
	return (new);
}

int		insert_env(t_group *grp, char *env)
{
	t_env	*new;
	int		i;

	i = 0;
	env = ft_strtrim(env);
	new = create_env_line(grp, env, i);
	if (new == NULL)
		return (-1);
	if (new->exist == true)
		return (1);
	if (grp->last != NULL)
		grp->last->next = new;
	else
		grp->first = new;
	grp->last = new;
	return (1);
}

t_group	*set_grp(void)
{
	t_group	*grp;
	struct winsize	w;
	int		i;

	i = -1;
	grp = (t_group *)malloc(sizeof(t_group));
	if (!(grp))
		exit(0);
	ioctl(0, TIOCGWINSZ, &w);
	grp->first = NULL;
	grp->last = NULL;
	grp->cmd = NULL;
	grp->curr_cmd = NULL;
	grp->cmd_save = NULL;
	grp->env = NULL;
	grp->hist = NULL;
	grp->quotes = (int *)malloc(sizeof(int) * 6);
	while (++i < 6)
		grp->quotes[i] = 0;
	grp->curr_hist = NULL;
	grp->fd_in_save = STDIN_FILENO;
	//grp->fd_out_save = open("/tmp/.tmp_file", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	grp->pipe = 0;
	grp->exit[0] = 0;
	grp->exit[1] = 0;
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
	grp->term = (t_term *)malloc(sizeof(t_term));
	grp->term->curs_pos = 0;
	grp->term->cmd_size = 0;
	grp->term->other_read = 0;
	grp->term->cmd_line = NULL;
	grp->term->window = (t_window *)malloc(sizeof(t_window));
	grp->term->window->width = w.ws_col;
	grp->term->window->heigth = w.ws_row;
	return (grp);
}

t_group	*init_grp(void)
{
	static t_group *grp = NULL;

	if (grp == NULL)
		grp = set_grp();
	return (grp);
}
