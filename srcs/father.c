/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   father.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 23:53:16 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/29 19:15:26 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	**get_all_command(int const fd)
{
	char	*cmd_line;
	char	*order;
	int		ret;

	ret = 0;
	order = NULL;
	get_next_line(fd, &order);
	cmd_line = SDUP(order);
	while ((ret = get_next_line(fd, &order)) > 0)
	{
		cmd_line = JOIN(cmd_line, ";");
		cmd_line = JOIN(cmd_line, order);
	}
	return (ft_strsplit(cmd_line, ';'));
}

char	**get_command(t_group *grp ,int const fd)
{
	char	*order;

	order = SDUP("");
	ft_putstr_fd("\033[1;32m", 2);
	ft_putstr_fd("fsh-> ", 2);
	ft_putstr_fd("\033[1;37m", 2);
	read_cmd(grp, fd, &order);
	return (ft_strsplit(order, ';'));
}

char	*check_shortcut(t_group *grp, char *order)
{
	int		i;
	char	*tmp;

	i = ft_strintchr(order, '~');
	if (i >= 0 && ft_getenv(grp, "HOME") == NULL)
		ft_putendl("HOME has been unset from environnement !");
	else if (i >= 0)
	{
		tmp = SUB(order, i + 1, LEN(order));
		order = SUB(order, 0, i);
		tmp = JOIN(ft_getenv(grp, "HOME"), tmp);
		order = JOIN(order, tmp);
	}
	return (order);
}

void	reset_opt(t_group *grp)
{
	int	i;

	i = -1;
	while (++i < 4)
		grp->define_cmd[i] = false;
	i = -1;
	while (++i < 7)
	{
		grp->options->on[i] = false;
		REMOVE(&grp->options->params[i]);
	}
	grp->pipe = 0;
}

void	parse_cmd(int const fd, t_group *grp)
{
	char	**all_cmd;
	char	*path;
	int		i;
	int		j;

	i = -1;
	all_cmd = fd == 0 ? get_command(grp, fd) : get_all_command(fd);
	while (all_cmd[++i])
	{
		(j = -1) ? reset_opt(grp) : 0;
		all_cmd[i] = check_shortcut(grp, all_cmd[i]);
		grp->cmd = ft_spacesplit(all_cmd[i]);
		while (grp->cmd[++j])
			grp->cmd[j] = ft_strtrim(grp->cmd[j]);
		if (j > 0)
		{
			all_cmd[i] = ft_strtrim(all_cmd[i]);
			grp->order = SDUP(all_cmd[i]);
			if (check_synth_cmd(grp) >= 0)
				create_process(grp);
		}
	}
}
