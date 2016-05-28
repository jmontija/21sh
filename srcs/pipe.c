/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/28 18:48:32 by jmontija         ###   ########.fr       */
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

int		check_pipe(t_group *grp, char cmd, int synth)
{
	if (synth == 0 && cmd == '|')
	{
		synth = 1;
		grp->pipe += 1;
	}
	else if (synth == 1 && cmd != '|')
		synth = 0;
	else if (synth == 1 && cmd == '|')
		synth = -1;
	return (synth);
}

void	finalize_cmd(t_group *grp, int synth)
{
	int		i;
	char	*order;

	while (synth)
	{
		order = SDUP("");
		ft_putstr_fd("\033[1;34m", 2);
		ft_putstr_fd("pipe> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
		read_cmd(grp, 0, &order);
		grp->order = ft_charjoin(grp->order, '\n');
		grp->order = JOIN(grp->order, order);
		i = -1;
		while (order[++i])
		{
			synth = check_pipe(grp, order[i], synth);
			if (synth < 0)
				error_synthax("error parsing near", "|");
		}
		REMOVE(&order);
	}
	printf("ORDER = %s\n", grp->order);
}

int		check_synth(t_group *grp, char **split_cmd)
{
	int			i;
	char		*path;
	char		**pipe_cmd;
	struct stat	s_buf;

	i = -1;
	while (split_cmd[++i])
	{
		if (split_cmd[i][0] == '>' || split_cmd[i][0] == '<')
			error_synthax("error parsing near", "|");
		pipe_cmd = ft_spacesplit(get_cmd(grp, split_cmd[i]));
		path = search_exec(grp, pipe_cmd[0]);
		if (lstat(path, &s_buf) < 0)
			error_synthax("unknown command", pipe_cmd[0]);
		if (i == grp->pipe)
			return (0);
	}
	if (i < grp->pipe)
		error_synthax("error parsing near", "|");
	finalize_cmd(grp, 1);
	ft_parsing(1, grp->order);
	return (-1);
}

int		main_pipe(t_group *grp, char **split_cmd)
{
	int	i;

	i = -1;
	if (check_synth(grp, split_cmd) < 0)
		return (0);
	while (split_cmd[++i])
	{
		grp->curr_pipe_cmd = SDUP(split_cmd[i]);
		printf("CURR_PIPE_CMD = %s\n", split_cmd[i]);
		grp->curr_cmd = get_cmd(grp, split_cmd[i]);
		ft_parsing(1, split_cmd[i]);
		if (i == grp->pipe)
		{
			if (exec_redir(1, grp, grp->curr_cmd) < 0)
			{
				grp->pipe = 0;
				dup2(grp->fd_in_save, STDIN_FILENO);
				split_exec_cmd(grp, grp->curr_cmd, "LAST COMMAND TO EXEC BY PIPE -> ");
				REMOVE(&grp->curr_cmd);
			}
		}
		else
			create_pipe(grp, grp->curr_cmd);
		REMOVE(&grp->curr_pipe_cmd);
	}
	return (0);
}
