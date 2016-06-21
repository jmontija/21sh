/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/05 03:07:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/20 16:09:12 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	create_process_env(t_group *grp, char *path, char **cmd_line)
{
	pid_t	pid;
	int		buf;
	int		fd;

	if (path == NULL)
		return ;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		buf == SIGSEGV ? error_cmd("segmentation fault", cmd_line[0]) : 0;
	}
	else if (pid == 0)
	{
		grp->options->on[ii] ? update_env(grp) : 0;
		dup2(grp->fd_in_save, STDIN_FILENO);
		execve(path, cmd_line, grp->env) < 1 ?
		ft_putendl_fd("execve failed", 2) : 0;
		exit(0);
	}
}

int		exec_cmd(t_group *grp, char *path, char *cmd_line)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd("env: unknown command", cmd_line);
	else if (s_buf.st_size <= 0)
		error_cmd("env: executable format error", cmd_line);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("env: Permission denied", cmd_line);
	else
		return (1);
	return (-1);
}

int		check_env_var(t_group *grp)
{
	char	**modif;
	char	*env;
	int		ret;
	int		i;
	int		j;

	i = -1;
	ret = 0;
	modif = ft_strsplit("LANG LC_ALL LC_CTYPE LC_MESSAGES NLSPATH PATH", ' ');
	while (++i < grp->define_cmd[namenv] &&
		CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1])
	{
		j = -1;
		env = CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1];
		while (modif[++j] != NULL)
		{
			if (ft_strncmp(env, modif[j], LEN(modif[j])) == 0)
			{
				ret =
				insert_env(grp, CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1]);
				break ;
			}
		}
	}
	return (ret);
}

char	*env_cmd(t_group *grp, int opt)
{
	char	*path;
	int		cmd_nb;
	int		i;

	list_to_tab(1, grp->first, &(grp->env_save));
	i = check_env_var(grp);
	list_to_tab(1, grp->first, &(grp->env));
	cmd_nb = (opt == P) ? (grp->define_cmd[utils] + 1) : grp->define_cmd[utils];
	grp->cmd = grp->cmd + cmd_nb;
	if (opt == P)
		grp->cmd[0] = SDUP(grp->options->params[P]);
	if (grp->cmd[0][0] == '.' || grp->cmd[0][0] == '/')
		path = SDUP(grp->cmd[0]);
	else
		path = search_exec(grp, grp->cmd[0]);
	if (i == 1)
	{
		i = -1;
		while (grp->env_save[++i] != NULL)
			insert_env(grp, grp->env_save[i]);
	}
	if (exec_cmd(grp, path, grp->cmd[0]) < 0)
		return (NULL);
	return (path);
}

void	exec_env(t_group *grp, int display)
{
	char	*path;
	int		bin;

	if (display == true)
		return (show_env(grp));
	bin = grp->define_cmd[utils];
	if (grp->options->on[P] == false && grp->options->on[u] == false)
	{
		path = env_cmd(grp, v);
		grp->options->on[v] == true ? opt_v(grp, grp->cmd) : 0;
		create_process_env(grp, path, grp->cmd);
	}
	else if (grp->options->on[P] == true)
	{
		path = env_cmd(grp, P);
		grp->options->on[v] == true ? opt_v(grp, grp->cmd) : 0;
		create_process_env(grp, path, grp->cmd);
	}
}
