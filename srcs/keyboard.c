/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:05:11 by jmontija          #+#    #+#             */
/*   Updated: 2016/04/30 00:21:55 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	print_cmd(t_group *grp, int key, char **cmd)
{
	char	*beg_cmd;
	char	*end_cmd;

	ft_putchar_fd(key, 2);
	if (grp->curs_pos < START_POS + LEN(*cmd))
	{
		beg_cmd = SUB(*cmd, 0, grp->curs_pos - START_POS);
		beg_cmd = ft_charjoin(beg_cmd, key);
		end_cmd = SUB(*cmd, grp->curs_pos - START_POS, LEN(*cmd));
		ft_putstr_fd(end_cmd, 2);
		ft_tputs(NULL, "ch");
		*cmd = JOIN(beg_cmd, end_cmd);
	}
	else
		*cmd = ft_charjoin(*cmd, key);
	grp->curs_pos += 1;
}

void	remove_line(t_group *grp, char **cmd)
{
	size_t	i;

	i = -1;
	grp->curs_pos = START_POS + LEN(*cmd) - 1;
	ft_tputs(NULL, "ch");
	while (++i < LEN(*cmd))
	{
		ft_tputs("le", NULL);
		ft_tputs("dc", NULL);
	}
	REMOVE(cmd);
	*cmd = SDUP("");
	grp->curs_pos = START_POS;
}

int		key_selection(t_group *grp, int key, char **cmd)
{
	if (grp && key == ENTER)
		return (ENTER);
	else if (key == BACKSPACE)
		handling_backspace(grp, cmd);
	else if (key == DEL)
		remove_line(grp, cmd);
	else if (key == ARROW_L || key == ARROW_R || key == ARROW_U || key == ARROW_D)
		handling_arrow(grp, cmd, key);
	else if (ft_isprint(key))
		print_cmd(grp, key, cmd);
	return (0);
}

void	read_cmd(t_group *grp, int fd, char **cmd)
{
	int		ret;
	int		key;
	char	order[BUF_SIZE + 1];

	ft_bzero(order, BUF_SIZE + 1);
	while ((ret = read(fd, order, BUF_SIZE)))
	{
		key = KEY(order[0], order[1], order[2], order[3]);
		if (key == CTRL_D || key == ESC)
		{
			reset_shell();
			ft_putchar('\n');
			exit(0);
		}
		else if (key_selection(grp, key, cmd) == ENTER)
		{
			grp->curs_pos = START_POS;
			ft_putchar('\n');
			break ;
		}
		ft_bzero(order, BUF_SIZE + 1);
	}
}