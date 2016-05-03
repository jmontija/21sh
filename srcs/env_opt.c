/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/05 03:07:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/03 17:17:35 by jmontija         ###   ########.fr       */
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
	ft_putendl("\n\t\t\tFSH_ENVIRONNEMENT\n");
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

void	opt_v(t_group *grp, char **cmd)
{
	int	i;

	i = -1;
	grp->options->on[ii] ? ft_putendl("#env clearing -> environ") : 0;
	ft_putstr("#env executing -> ");
	ft_putendl(cmd[0]);
	while (cmd[++i] != NULL)
	{
		ft_putstr("#env arg[");
		ft_putnbr(i);
		ft_putstr("] -> ");
		ft_putendl(cmd[i]);
	}
}

char	**env_cmd(t_group *grp, int opt)
{
	char	**cmd;
	int		cmd_nb;
	int		i;

	i = -1;
	cmd = NULL;
	cmd_nb = (opt == P) ? (grp->define_cmd[utils] + 1) : grp->define_cmd[utils];
	if (ft_getenv(grp, "PATH") != NULL)
		cmd = ft_spacesplit(ft_strstr(grp->order, grp->cmd[cmd_nb]));
	else
		cmd = ft_spacesplit(grp->order);
	while (cmd[++i])
		cmd[i] = ft_strtrim(cmd[i]);
	return (cmd);
}

void	exec_env(t_group *grp, int display)
{
	char	*path;
	char	**cmd;
	int		bin;

	if (display)
		return (show_env(grp));
	bin = grp->define_cmd[utils];
	if (grp->options->on[P] == false && grp->options->on[u] == false)
	{
		cmd = env_cmd(grp, v);
		grp->options->on[v] == true ? opt_v(grp, cmd) : 0;
		path = search_exec(grp, grp->cmd[bin]);
		if (grp->cmd[bin][0] == '.' || grp->cmd[bin][0] == '/')
			path = SDUP(grp->cmd[bin]);
		exec_cmd(grp, path, cmd);
	}
	else if (grp->options->on[P] == true)
	{
		cmd = env_cmd(grp, P);
		path = ft_getenv(grp, "PATH") == NULL ?
		ft_getenv(grp, "_") : SDUP(grp->options->params[P]);
		grp->options->on[v] == true ? opt_v(grp, cmd) : 0;
		exec_cmd(grp, path, cmd);
	}
}
