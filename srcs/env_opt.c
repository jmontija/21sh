/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/05 03:07:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/28 17:51:12 by jmontija         ###   ########.fr       */
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

void	create_process_env(t_group *grp, char *path, char **cmd_line)
{
	pid_t	pid;
	int		buf;
	int		fd;

	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		buf == SIGSEGV ? error_cmd("segmentation fault", cmd_line[0]) : 0;
	}
	else if (pid == 0 && execve(path, cmd_line, grp->env) < 1)
	{
		(fd = open(path, O_RDONLY)) != -1 ?
		parse_cmd(fd, grp) : error_cmd("unknown command", cmd_line[0]);
		exit(0);
	}
}

void	exec_env(t_group *grp, int display)
{
	char	*path;
	char	**cmd;
	int		bin;

	if (display)
		return (show_env(grp));
	path = search_exec(grp, grp->cmd[0]);
	if (grp->cmd[0][0] == '.' || grp->cmd[0][0] == '/')
		path = SDUP(grp->cmd[0]);
	if (exec_cmd(grp, path, grp->cmd) > 0)
		create_process_env(grp, path, grp->cmd);
}
