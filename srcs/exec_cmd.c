/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/27 17:14:40 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/27 20:20:56 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	is_parenthese_closed(t_group *grp, int synth)
{
	char	*order;
	int		i;

	while (synth)
	{
		order = SDUP("");
		ft_putstr_fd("\033[1;34m", 2);
		ft_putstr_fd("quote> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
		read_cmd(grp, 0, &order);
		grp->order = ft_charjoin(grp->order, '\n');
		grp->order = JOIN(grp->order, order);
		i = -1;
		while (order[++i])
			synth = check_parenthese(order[i], synth);
		REMOVE(&order);
	}
	printf("ORDER = %s\n", grp->order);
}

int		launch_parser(t_group *grp)
{
	int	i;
	int ret;
	int synth;

	i = -1;
	synth = false;
	while (grp->order[++i] != '\0')
	{
		synth = check_parenthese(grp->order[i], synth);
		if (synth == false && grp->order[i] == '|')
			grp->pipe += 1;
	}
	is_parenthese_closed(grp, synth);
	grp->curr_cmd = get_cmd(grp, grp->order);
	ret = ft_parsing(1, grp->order);
	if (ret < 0)
		ret = exec_redir(1, grp, grp->curr_cmd);
	if (ret < 0)
	{
		ret = exec_builtin(0, grp, grp->order);
		printf("RET = %d\n", ret);
		if (ret <= 0)
			split_exec_cmd(grp, grp->curr_cmd, "COMMAND TO EXEC BASIC WAY ");
	}
	return (ret);
}

void	manage_env(t_group *grp)
{
	int		i;
	int		pos;
	char	**env;

	i = -1;
	pos = 0;
	env = (char **)malloc(sizeof(char *) * 100);
	env[100] = NULL;
	grp->options->on[ii] == false ? pos = list_to_tab(grp->first, &env) : 0;
	while (++i < grp->define_cmd[namenv] &&
		grp->cmd[i + grp->define_cmd[e_opt] + 1])
		env[pos + i] = SDUP(grp->cmd[i + grp->define_cmd[e_opt] + 1]);
	grp->env = env;
	// A REVOIR CETTE LIGNE DE COMMANDE EST SUREMENT OUTDATED POUR 21SH
}

void	create_process(t_group *grp)
{
	pid_t	pid;
	int		buf;
	int		fd;
	char	**split_cmd;
	char	*splitw;
	int 	i = 0;

	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid != 0)
	{
		manage_env(grp);
		waitpid(pid, &buf, 0);
		buf == SIGSEGV ? error_cmd("segmentation fault", grp->curr_cmd) : 0;
		if ((splitw = ft_findocc(false, grp->order, "| >> > << < 1>&2 2>&1 2>&- 1>&- >&-")) == NULL)
			splitw = SDUP("|");
		split_cmd = ft_strsplitstr(grp->order, splitw);
		while (split_cmd[i])
			i++;
		if (splitw[0] == '|')
			split_cmd = ft_spacesplit(split_cmd[i - 1]);
		else
			split_cmd = ft_spacesplit(split_cmd[0]);
		i = -1;
		while (split_cmd[++i])
		{
			ft_putendl_fd(JOIN("BUILTIN TO MAKE -> ", split_cmd[i]), 2);
			split_cmd[i] = ft_strtrim(split_cmd[i]);
		}
		grp->cmd = split_cmd;
		exec_builtin(1, grp, split_cmd[i - 1]);
	}
	else if (pid == 0)
	{
		//manage_env(grp);
		launch_parser(grp);
		exit(0);
	}
}

void	create_process_env(t_group *grp, char *path, char **cmd_line)
{
	pid_t	pid;
	int		buf;
	int		fd;

	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		buf == SIGSEGV ? error_cmd("segmentation fault", cmd_line[0]) : 0;
	}
	else if (pid == 0 && execve(path, cmd_line, grp->env) < 1)
	{
		(fd = open(path, O_RDONLY)) != -1 ?
		parse_cmd(fd, grp) : error_cmd("unknown command", cmd_line[0]);
		exit(0);
	}
}

void	exec_cmd(t_group *grp, char *path, char **cmd_line)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd("unknown command", cmd_line[0]);
	else if (s_buf.st_size <= 0)
		error_cmd("executable format error", cmd_line[0]);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("Permission denied", cmd_line[0]);
	else
		create_process_env(grp, path, cmd_line);
}
