/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/15 19:20:16 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	create_pipe(t_group *grp, char *pipe_cmd)
{
	pid_t		pid;
	int			buf;
	int			fd[2];

	ft_putendl("create_pipe");
	pipe(fd) != 0 ? ft_putendl("error pipe function") : 0;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		exec_redir(1, grp, pipe_cmd);
		dup2(grp->fd_in_save, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		split_exec_cmd(grp, pipe_cmd, "COMMAND TO EXEC BY PIPE -> ");
	}
	else if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		close(fd[1]);
		grp->fd_in_save = fd[0];
		exec_redir(0, grp, pipe_cmd); // <- pour free
	}
}

int		main_pipe(t_group *grp, char **split_cmd)
{
	int	i;

	i = -1;
	while (split_cmd[++i])
	{
		ft_putendl(split_cmd[i]);
		grp->curr_cmd = get_cmd(grp, split_cmd[i]);
		ft_parsing(1, split_cmd[i]);
		printf("I = %d, PIPE = %d\n", i, grp->pipe);
		if (i == grp->pipe)
		{

			if (exec_redir(1, grp, grp->curr_cmd) < 0)
			{
				grp->pipe = 0;
				dup2(grp->fd_in_save, STDIN_FILENO);
				split_exec_cmd(grp, grp->curr_cmd, "COMMAND TO EXEC BY PIPE -> ");
				REMOVE(&grp->curr_cmd);
			}
		}
		else
			create_pipe(grp, grp->curr_cmd);
	}
	return (0);
}
