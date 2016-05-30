/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/27 17:14:40 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/30 20:25:30 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		launch_parser(t_group *grp)
{
	int ret;

	grp->curr_cmd = get_cmd(grp, grp->order);
	ret = ft_parsing(1, grp->order);
	if (ret < 0)
		ret = exec_redir(1, grp, grp->curr_cmd);
	if (ret < 0)
	{
		ret = exec_builtin(0, grp, grp->order);
		//printf("RET = %d\n", ret);
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

	manage_env(grp);
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		buf == SIGSEGV ? error_cmd("segmentation fault", grp->curr_cmd) : 0;
		manage_redirections(0, grp, NULL);
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
		if (ft_findocc(false, grp->order, "| >> > << < 1>&2 2>&1 2>&- 1>&- >&-") == NULL
			|| ft_strcmp(split_cmd[0], "env") != 0)
			exec_builtin(1, grp, split_cmd[i - 1]);
	}
	else if (pid == 0)
	{
		launch_parser(grp);
		exit(0);
	}
}
