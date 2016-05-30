/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/30 20:32:37 by jmontija         ###   ########.fr       */
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
		manage_redirections(1, grp, pipe_cmd);
		dup2(fd[1], STDOUT_FILENO);
		split_exec_cmd(grp, SDUP("cat tmp"), "COMMAND TO EXEC BY PIPE -> ");
		close(fd[0]);
	}
	else if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		manage_redirections(0, grp, pipe_cmd); // <- pour free
		grp->fd_in_save = fd[0];
		close(fd[1]);
	}
}

int		main_pipe(t_group *grp, char **split_cmd)
{
	int	i;

	i = -1;
	while (split_cmd[++i])
	{
		grp->curr_pipe_cmd = SDUP(split_cmd[i]);
		//printf("CURR_PIPE_CMD = %s\n", split_cmd[i]);
		grp->curr_cmd = get_cmd(grp, split_cmd[i]);
		ft_parsing(1, split_cmd[i]);
		if (i == grp->pipe)
		{
			if (manage_redirections(1, grp, grp->curr_cmd) <= 0)
			{
				grp->pipe = 0;
				split_exec_cmd(grp, SDUP("cat tmp"), "LAST COMMAND TO EXEC BY PIPE -> ");
				REMOVE(&grp->curr_cmd);
			}
		}
		else
			create_pipe(grp, grp->curr_cmd);
		REMOVE(&grp->curr_pipe_cmd);
	}
	return (0);
}
