/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 18:04:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/20 15:30:23 by jmontija         ###   ########.fr       */
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
}

void	exec_fils(int fd[2], t_group *grp, int redir_all)
{
	int		fd_cmd;
	char	*cmd_spec[5];

	dup2(grp->fd_in_save, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	redir_all ? dup2(fd[1], STDERR_FILENO) : 0;
	if (CMD(fd) == 1 && execve(CMD(name), CMD(cmd_split), grp->env) < 1)
	{
		fd_cmd = open(CMD(name), O_RDONLY);
		dup2(fd_cmd, grp->fd_in_save);
		cmd_spec[0] = "./21sh";
		cmd_spec[1] = NULL;
		if (execve("./21sh", cmd_spec, grp->env))
			ft_putendl("execve failed");
		close(fd_cmd);
		exit(0);
	}
	else if (fd[1] > 2 && CMD(fd) == 0)
	{
		grp->cmd = CMD(cmd_split);
		exec_builtin(1, grp, NULL);
		grp->cmd = NULL;
	}
	close(fd[0]);
	exit(0);
}

void	create_pipe(t_group *grp)
{
	pid_t		pid;
	int			fd[2];

	pipe(fd) != 0 ? ft_putendl("error pipe function") : 0;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
		exec_fils(fd, grp, 0);
	else if (pid != 0)
	{
		close(fd[1]);
		grp->fd_in_save = fd[0];
	}
}

int		exec_command(int fd, t_group *grp, int redir_all)
{
	pid_t		pid;
	int			buf;
	int			fd_fils[2];

	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		fd_fils[0] = fd;
		fd_fils[1] = fd;
		exec_fils(fd_fils, grp, redir_all);
	}
	else if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		if (CMD(fd) == 0 && (fd == 1 || ft_strcmp("env", CMD(cmd_split)[0])))
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
	reinitialized(grp);
	return (ret);
}
