/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lib2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/19 01:30:57 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/19 01:31:47 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	show_or_overwrite(t_group *grp, t_env *curr)
{
	t_bool	overwrite;
	char	*key;
	char	*value;
	int		i;
	int		j;

	i = -1;
	overwrite = false;
	if (grp->options->params[u] == NULL ||
		ft_strcmp(grp->options->params[u], curr->name) != 0)
	{
		ft_putstr(curr->name);
		ft_putchar('=');
		overwrite = false;
		while (++i < grp->define_cmd[namenv] &&
			grp->cmd[i + grp->define_cmd[e_opt] + 1])
		{
			j = ft_strintchr(grp->cmd[i + grp->define_cmd[e_opt] + 1], '=');
			key = SUB(grp->cmd[i + grp->define_cmd[e_opt] + 1], 0, j);
			value = STRCHR(grp->cmd[i + grp->define_cmd[e_opt] + 1], '=') + 1;
			if (ft_strcmp(key, curr->name) == 0)
				(overwrite = true) ? ft_putendl(value) : 0;
		}
		overwrite == false ? ft_putendl(curr->val) : 0;
	}
}

void	show_env(t_group *grp)
{
	t_env	*curr;
	char	*key;
	int		i;
	int		j;

	i = -1;
	curr = grp->first;
	while (curr != NULL && grp->options->on[ii] == false)
	{
		show_or_overwrite(grp, curr);
		curr = curr->next;
	}
	while (++i < grp->define_cmd[namenv] &&
		grp->cmd[i + grp->define_cmd[e_opt] + 1])
	{
		j = ft_strintchr(grp->cmd[i + grp->define_cmd[e_opt] + 1], '=');
		key = SUB(grp->cmd[i + grp->define_cmd[e_opt] + 1], 0, j);
		if (grp->options->on[ii] || ft_getenv(grp, key) == NULL)
			ft_putendl(grp->cmd[i + grp->define_cmd[e_opt] + 1]);
	}
}

void	update_env(t_group *grp)
{
	int	i;
	int	pos;

	pos = 0;
	i = -1;
	while (grp->env && grp->env[++i] != NULL)
		REMOVE(&(grp->env[i]));
	free(grp->env);
	grp->env = NULL;
	i = -1;
	while (++i < grp->define_cmd[namenv] &&
		CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1])
		pos += 1;
	grp->env = (char **)malloc(sizeof(char *) * (pos + 1));
	i = -1;
	while (++i < pos + 1)
		grp->env[i] = NULL;
	i = -1;
	while (++i < (grp->define_cmd[namenv] - 1) &&
		CMD(cmd_split)[i + grp->define_cmd[e_opt]])
		grp->env[i] = SDUP(CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1]);
}

void	opt_v(t_group *grp, char **cmd)
{
	int	i;

	i = -1;
	grp->options->on[ii] ? ft_putendl_fd("#env clearing -> environ", 2) : 0;
	ft_putstr_fd("#env executing -> ", 2);
	ft_putendl_fd(cmd[0], 2);
	while (cmd[++i] != NULL)
	{
		ft_putstr_fd("#env arg[", 2);
		ft_putnbr_fd(i, 2);
		ft_putstr_fd("] -> ", 2);
		ft_putendl_fd(cmd[i], 2);
	}
}
