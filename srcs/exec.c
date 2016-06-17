/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/17 03:34:03 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	reinitialized(t_group *grp)
{
	int	i;

	i = -1;
	while (grp->env && grp->env[++i] != NULL)
		REMOVE(&(grp->env[i]));
	free(grp->env);
	grp->env = NULL;
	i = -1;
	while (grp->env_save && grp->env_save[++i] != NULL)
		REMOVE(&(grp->env_save[i]));
	free(grp->env_save);
	grp->env_save = NULL;
	i = -1;
	while (++i < 4)
		grp->define_cmd[i] = false;
	i = -1;
	while (++i < 7)
	{
		grp->options->on[i] = false;
		REMOVE(&grp->options->params[i]);
	}
	unlink(TMP_FROM);
	//ft_putendl_fd("env OPTIONS REINITIALIZED", 2);
	//ft_putendl("grp->env FREE");
}

void	create_pipe(t_group *grp)
{
	pid_t		pid;
	int			buf;
	//int			fd[2];

	//pipe(fd) != 0 ? ft_putendl("error pipe function") : 0;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		dup2(grp->fd_in_save, STDIN_FILENO);
		grp->fd_out_save = open("/dev/tty1", O_TRUNC);
		dup2(grp->fd_out_save, STDOUT_FILENO);
		//close(fd[0]);
		if (CMD(fd) == 1)
			execve(CMD(name), CMD(cmd_split), grp->env) < 1 ? ft_putendl_fd("execve failed", 2) : 0;
		else if (CMD(fd) == 0)
		{
			grp->cmd = CMD(cmd_split);
			exec_builtin(1, grp, NULL);
			grp->cmd = NULL;
		}
		exit(0);
	}
	else if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		grp->fd_in_save = open("/dev/tty1", O_APPEND);
	}
}

int		exec_command(int fd, t_group *grp, int redir_all)
{
	pid_t		pid;
	int			buf;

	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		dup2(grp->fd_in_save, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		redir_all ? dup2(fd, STDERR_FILENO) : 0;
		if (CMD(fd) == 1)
			execve(CMD(name), CMD(cmd_split), grp->env) < 1 ? ft_putendl_fd("execve failed", 2) : 0;
		else if (fd > 1 && CMD(fd) == 0)
		{
			grp->cmd = CMD(cmd_split);
			exec_builtin(1, grp, NULL);
			grp->cmd = NULL;
		}
		close(fd);
		exit(0);
	}
	else if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		if (fd == 1)
		{
			grp->cmd = CMD(cmd_split);
			exec_builtin(1, grp, NULL);
			grp->cmd = NULL;
		}
	}
	return (true);
}

int		main_pipe(t_group *grp, char **split_cmd)
{
	int	i;
	int	ret;
	int	exec;

	i = -1;
	ret = 0;
	reset_shell();
	while (grp->sh_cmd[++i] != NULL)
	{
		exec = manage_pre_exec(i, grp, NULL);
		if (grp->exit[1] == true && (ret = -1))
			break ;
		if (grp->sh_cmd[i + 1] == NULL)
		{
			if (exec == 0)
				exec_command(STDOUT_FILENO, grp, 0);
		}
		else
			create_pipe(grp);
		if (grp->exit[1] == true && (ret = -1))
			break ;
		reinitialized(grp);
	}
	set_shell((~ICANON & ~ECHO));
	reinitialized(grp);
	return (ret);
}
