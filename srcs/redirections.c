/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/05 13:51:38 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
	<> file -> make file
	cmd (> < >> <<) file [file1 file2 ...]options_cmd
	traité de gauche a droite
		-> cmd < file2 <file1 > file4 < file3  >file5
		-> cmd get info from file2 file1 file3 and redirect it to file4 file5 etc .. till next_cmd
*/

int 	fd_save = 0;

void	create_pipe(char *path, char **cmd_line, char **env, int idx)
{
	t_group *grp = init_grp();
	pid_t		pid;
	int			buf;
	char		**pipe_cmd;
	int			fd[2];
	int			j;

	ft_putendl("create_pipe");
	pipe(fd) != 0 ? ft_putendl("error pipe function") : 0;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	j = -1;
	if (pid == 0)
	{
		printf("FILS\n");
		dup2(fd_save, 0);
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
		printf("PEREWAIT\n");
		waitpid(pid, &buf, 0);
		printf("PEREACT\n");
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		fd_save = fd[0];
		if (redirections(path, cmd_line + 2, env) == false)
		{
			pipe_cmd = ft_strsplit(cmd_line[idx + 1], '/');
			while (pipe_cmd[++j])
				pipe_cmd[j] = ft_strtrim(pipe_cmd[j]);
			execve(search_exec(grp, pipe_cmd[0]), pipe_cmd, env) < 1 ? ft_putendl("error pipe execve") : 0;
			exit(0);
		}
	}
}

char	**create_redirection_to(char **cmd_line, int idx, int action)
{
	int	fd;

	if (cmd_line[idx + 2] == NULL)
	{
		error_cmd("error pars near", cmd_line[idx + 1]);
		return (NULL);
	}
	fd = open(cmd_line[idx + 2], O_WRONLY | action | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
	close(fd);
	while (cmd_line[++idx])
		REMOVE(&cmd_line[idx]);
	return (cmd_line);
}

char	**create_redirection_from(char **cmd_line, int idx, int action)
{
	int	fd;

	if (cmd_line[idx + 2] == NULL)
	{
		error_cmd("error pars near", cmd_line[idx + 1]);
		return (NULL);
	}
	fd = open(cmd_line[idx + 2], O_RDONLY);
	dup2(fd, STDIN_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
	close(fd);
	while (cmd_line[++idx])
		REMOVE(&cmd_line[idx]);
	return (cmd_line);
}

int		redirections(char *path, char **cmd_line, char **env)
{
	int	occ;
	int i;
	int	match;

	i = -1;
	occ = -1;
	match = false;
	while (cmd_line[++i])
	{
		ft_putstr("cmd = ");
		ft_putendl(cmd_line[i]);
		if ((occ = ft_strintchr(cmd_line[i], '>')) >= 0)
		{
			if (occ == 0 && cmd_line[i][occ + 1] == '>')
				cmd_line = create_redirection_to(cmd_line, i - 1, O_APPEND);
			else if (occ == 0)
				cmd_line = create_redirection_to(cmd_line, i - 1, O_TRUNC);
			execve(path, cmd_line, env) < 1 ? ft_putendl("error pipe execve") : (match = true);
		}
		else if ((occ = ft_strintchr(cmd_line[i], '<')) >= 0)
		{
			if (occ == 0 && cmd_line[i][occ + 1] == '<')
			{} // heredoc prompt a approfondir;
			else if (occ == 0)
				cmd_line = create_redirection_from(cmd_line, i - 1, O_TRUNC);
			execve(path, cmd_line, env) < 1 ? ft_putendl("error pipe execve") : (match = true);
		}
		else if ((occ = ft_strintchr(cmd_line[i], '|')) >= 0)
		{
			create_pipe(path, cmd_line, env, i);
			match = true;
		}
	}
	ft_putstr("exit");
	return (match);
}