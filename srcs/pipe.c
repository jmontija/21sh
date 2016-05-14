/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/14 00:04:08 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	exec_cmd_pipe(t_group *grp, char *cmd_to_exec)
{
	char	**exec_cmd;
	int		i;

	i = -1;
	exec_cmd = ft_spacesplit(cmd_to_exec);
	while (exec_cmd[++i])
	{
		ft_putendl_fd(JOIN("COMMAND TO EXEC BY PIPE -> " ,exec_cmd[i]), 2);
		exec_cmd[i] = ft_strtrim(exec_cmd[i]);
	}
	execve(search_exec(grp, exec_cmd[0]), exec_cmd, NULL) < 1 ? ft_putendl_fd("error pipe execve", 2) : 0;
	// env a placer a la place de NULL le stocker dans grp->env !
	exit(0);
}

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
		exec_cmd_pipe(grp, pipe_cmd);
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
	int		i;
	char	*cmd_first;

	i = -1;
	cmd_first = NULL;
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
				exec_cmd_pipe(grp, grp->curr_cmd);
				REMOVE(&grp->curr_cmd);
			}
		}
		else
			create_pipe(grp, grp->curr_cmd);
		cmd_first = get_cmd(grp, split_cmd[i]);
	}
	return (1);
}
