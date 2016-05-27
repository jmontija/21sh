/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/23 17:29:45 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/27 20:19:40 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_setenv(t_group *grp)
{
	int		i;
	t_bool	error;

	i = 0;
	error = false;
	if (grp->cmd[1] == NULL)
	{
		ft_putendl("setenv: set argument(s) -> key=value or key=");
		return ;
	}
	while (grp->cmd[++i] != NULL)
	{
		if (insert_env(grp, grp->cmd[i]) < 0)
		{
			ft_putstr("setenv: bad synthax -> ");
			ft_putendl(grp->cmd[i]);
			error = true;
		}
	}
	error ? ft_putendl("setenv: synthax -> key=value or key=") : 0;
}

void	ft_unsetenv(t_group *grp)
{
	int		i;
	t_bool	error;

	i = 0;
	error = false;
	if (grp->cmd[1] == NULL)
	{
		ft_putendl("unsetenv: set argument(s) -> key");
		return ;
	}
	while (grp->cmd[++i] != NULL)
	{
		if (unset_env(grp, grp->cmd[i]) < 0)
		{
			ft_putstr("unsetenv: unfound key -> ");
			ft_putendl(grp->cmd[i]);
			error = true;
		}
	}
	error ? ft_putendl("unsetenv: synthax -> key") : 0;
}

void	manage_cd(t_group *grp)
{
	struct stat	s_buf;
	char		*path;

	if (grp->cmd[1] == NULL)
	{
		path = ft_getenv(grp, "HOME") ? ft_getenv(grp, "HOME") :
		SDUP("HOME has been unset from environnement !");
	}
	else if (grp->cmd[1][0] == '-' && grp->cmd[1][1] == false)
	{
		if ((path = ft_getenv(grp, "OLDPWD")) != NULL)
		{
			ft_putstr("> ");
			ft_putendl(path);
		}
		else
			path = SDUP("OLDPWD has been unset from environnement !");
	}
	else
		path = SDUP(grp->cmd[1]);
	lstat(path, &s_buf);
	cderr_pwd(grp, path, s_buf);
}

int		exec_builtin(int exec, t_group *grp, char *order)
{
	int		active;

	active = 0;
	if (ft_strcmp(grp->cmd[0], "env") == 0)
	{
		active += 1;
		if (exec == false || manage_opt(grp) < 0)
			return (active);
		(grp->options->on[u] == true ||
			(grp->define_cmd[namenv] > 0 && grp->define_cmd[utils] == false) ||
			grp->cmd[1] == NULL) ? exec_env(grp, 1) : exec_env(grp, 0);
	}
	else if (ft_strcmp(grp->cmd[0], "cd") == 0)
		((active += 1) && exec) ? manage_cd(grp) : 0;
	else if (ft_strcmp(grp->cmd[0], "setenv") == 0)
		((active += 1) && exec) ? ft_setenv(grp) : 0;
	else if (ft_strcmp(grp->cmd[0], "unsetenv") == 0)
		((active += 1) && exec) ? ft_unsetenv(grp) : 0;
	else if (ft_strcmp(grp->cmd[0], "exit") == 0)
		grp->cmd[1] ? exit(ft_atoi(grp->cmd[1])) : exit(0);
	return (active);
}
