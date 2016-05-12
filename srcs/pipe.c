/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/12 02:35:24 by julio            ###   ########.fr       */
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
		exec_redir(grp);
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
			//exec_redir(grp);
			exec_cmd_pipe(grp, pipe_cmd[1]);
		}
	}
}

char	*get_cmd(t_group *grp, char *cmd)
{
	char	**symbol;
	char	*shell_cmd;
	char	*tofind;
	size_t 	symlen;
	size_t		i;
	int			j;

	i = -1;
	tofind = SDUP("> >> < <<");
	symbol = ft_spacesplit(tofind);
	shell_cmd = SDUP(cmd);
	while (cmd[++i] != '\0')
	{
		j = -1;
		while (symbol[++j] != NULL)
		{
			symlen = ft_strlen(symbol[j]);
			if ( (symlen > 1 && strncmp(cmd + i, symbol[j], symlen) == 0) ||
				(symlen == 1 && *symbol[j] == cmd[i] && cmd[i - 1] && cmd[i + 1] &&
					cmd[i + 1] != *symbol[j] && cmd[i - 1] != *symbol[j]) )
			{
				shell_cmd = SDUP(ft_strsplitstr(cmd, symbol[j])[0]);
				return (shell_cmd);
			}
		}
	}
	return (shell_cmd);
}

void	pipe_manager(t_group *grp, char *cmd_first, char *cmd_second)
{
	char	*pipe_cmd[2];

	ft_putendl("PARS_DONE -> |");
	pipe_cmd[0] = SDUP(cmd_first);
	pipe_cmd[1] = get_cmd(grp, cmd_second);
	ft_putstr("first pipe_cmd -> ");
	ft_putendl(pipe_cmd[0]);
	ft_putstr("second pipe_cmd -> ");
	ft_putendl(pipe_cmd[1]);
	create_pipe(grp, pipe_cmd);
}
// RECUPERER LES COMMANDES PLUS PROPREMENT ET LES VERIFIER

int		main_pipe(t_group *grp, char **split_cmd)
{
	int		i;
	char	*cmd_first;

	i = -1;
	while (split_cmd[++i])
	{
		ft_putendl(split_cmd[i]);
		grp->curr_cmd = get_cmd(grp, split_cmd[i]);
		ft_parsing(1, split_cmd[i]);
		if (i > 0)
			pipe_manager(grp, cmd_first, split_cmd[i]);
		cmd_first = get_cmd(grp, split_cmd[i]);
	}
	return (1);
}
