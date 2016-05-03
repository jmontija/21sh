/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/27 17:14:40 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/03 18:07:39 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	create_process(t_group *grp, char *path, char **cmd_line, char **env)
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
	else
	{
		if ((cmd_line = redirections(cmd_line)) == NULL)
			exit(0);
		if (pid == 0 && execve(path, cmd_line, env) < 1)
		{
			(fd = open(path, O_RDONLY)) != -1 ?
			parse_cmd(fd, grp) : error_cmd("unknown command", cmd_line[0]);
			close(fd);
			exit(0);
		}
	}

}

void	manage_env(t_group *grp, char *path, char **cmd_line)
{
	int		i;
	int		pos;
	char	**env;

	i = -1;
	pos = 0;
	env = (char **)malloc(sizeof(char *) * 100);
	env[100] = NULL;
	grp->options->on[ii] == false ? pos = list_to_tab(grp->first, &env) : 0;
	while (++i < grp->define_cmd[namenv] &&
		grp->cmd[i + grp->define_cmd[e_opt] + 1])
		env[pos + i] = SDUP(grp->cmd[i + grp->define_cmd[e_opt] + 1]);
	create_process(grp, path, cmd_line, env);
}

void	exec_cmd(t_group *grp, char *path, char **cmd_line)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd("unknown command", cmd_line[0]);
	else if (s_buf.st_size <= 0)
		error_cmd("executable format error", cmd_line[0]);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("Permission denied", cmd_line[0]);
	else
		manage_env(grp, path, cmd_line);
}
