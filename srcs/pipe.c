/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/10 04:34:52 by julio            ###   ########.fr       */
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
			exec_cmd[i] = ft_strtrim(exec_cmd[i]);
	execve(search_exec(grp, exec_cmd[0]), exec_cmd, NULL) < 1 ? ft_putendl("error pipe execve") : 0;
	// env a placer a la place de NULL le stocker dans grp->env !
	exit(0);
}

void	create_pipe(t_group *grp, char **pipe_cmd)
{
	pid_t		pid;
	int			buf;
	int			fd[2];
	static int	pipe_counter = 0;

	ft_putendl("create_pipe");
	pipe_counter += 1;
	pipe(fd) != 0 ? ft_putendl("error pipe function") : 0;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		dup2(grp->fd_in_save, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		exec_cmd_pipe(grp, pipe_cmd[0]);
	}
	else if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		close(fd[1]);
		grp->fd_in_save = fd[0];
		if (grp->pipe == pipe_counter)
		{
			pipe_counter = 0;
			grp->pipe = 0;
			grp->curr_cmd = NULL;
			dup2(fd[0], STDIN_FILENO);
			exec_cmd_pipe(grp, pipe_cmd[1]);
		}
	}
}

// RECUPERER LES COMMANDES PLUS PROPREMENT 

void	pipe_manager(t_group *grp, char *cmd_first, char *cmd_second)
{
	char	*pipe_cmd[2];

	ft_putendl("PARS_DONE -> |");
	pipe_cmd[0] = SDUP(cmd_first);
	pipe_cmd[1] = SDUP(ft_strsplit(cmd_second, '>')[0]);
		if (ft_strcmp(pipe_cmd[1], cmd_second) == 0)
			pipe_cmd[1] = SDUP(ft_strsplit(cmd_second, '<')[0]);
	ft_putstr("first pipe_cmd -> ");
	ft_putendl(pipe_cmd[0]);
	ft_putstr("second pipe_cmd -> ");
	ft_putendl(pipe_cmd[1]);
	//make_redir();
	create_pipe(grp, pipe_cmd);
}
// RECUPERER LES COMMANDES PLUS PROPREMENT ET LES VERIFIER

char	*save_first_cmd(t_group *grp, char *cmd)
{
	grp->curr_cmd = SDUP(ft_strsplit(cmd, '>')[0]);
	if (ft_strcmp(grp->curr_cmd, cmd) == 0)
		grp->curr_cmd = SDUP(ft_strsplit(cmd, '<')[0]);
	return (grp->curr_cmd);
}

int		main_pipe(t_group *grp, char **split_cmd)
{
	int		i;
	char	*cmd_first;

	i = -1;
	while (split_cmd[++i])
	{
		ft_putendl(split_cmd[i]);
		save_first_cmd(grp, split_cmd[i]);
		ft_parsing(1, split_cmd[i]);
		if (i > 0)
			pipe_manager(grp, cmd_first, split_cmd[i]);
		cmd_first = save_first_cmd(grp, split_cmd[i]);
	}
	return (1);
}
