/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/07 20:39:48 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*void	create_pipe(char *path, char **cmd_line, char **env, int idx)
{
	t_group *grp = init_grp();
	pid_t		pid;
	int			buf;
	char		**pipe_cmd;
	int			fd[2];
	int			j;

	//ft_putendl("create_pipe");
	pipe(fd) != 0 ? ft_putendl("error pipe function") : 0;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	j = -1;
	if (pid == 0)
	{
		//printf("FILS\n");
		dup2(grp->fd_save, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		pipe_cmd = ft_strsplit(cmd_line[idx - 1], '/');
		while (pipe_cmd[++j])
			pipe_cmd[j] = ft_strtrim(pipe_cmd[j]);
		execve(search_exec(grp, pipe_cmd[0]), pipe_cmd, env) < 1 ? ft_putendl("error pipe execve") : 0;
		exit(0);
	}
	else if (pid != 0)
	{
		//printf("PEREWAIT\n");
		waitpid(pid, &buf, 0);
		//printf("PEREACT\n");
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		grp->fd_save = fd[0];
		pipe_cmd = ft_strsplit(cmd_line[idx + 1], '/');
		while (pipe_cmd[++j])
			pipe_cmd[j] = ft_strtrim(pipe_cmd[j]);
		if (ft_parsing(search_exec(grp, pipe_cmd[0]), cmd_line + 2, env) == false)
		{
			execve(search_exec(grp, pipe_cmd[0]), pipe_cmd, env) < 1 ? ft_putendl("error pipe execve") : 0;
			exit(0);
		}
	}
}*/

int		main_pipe(t_group *grp, char **split_cmd)
{
	printf("IN PIPE\n");
	return (1);
}