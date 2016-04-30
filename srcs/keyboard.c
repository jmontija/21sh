/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:05:11 by jmontija          #+#    #+#             */
/*   Updated: 2016/04/30 13:47:26 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	print_cmd(t_group *grp, int key, char *order, char **cmd)
{
	char	*beg_cmd = NULL;
	char	*end_cmd;
	int i = -1;

	while (order[++i] != '\0')
		ft_putchar_fd(order[i], 2);
	if (grp->curs_pos < START_POS + LEN(*cmd))
	{
		/*if (beg_cmd != NULL)
			beg_cmd = JOIN(beg_cmd, )*/
		beg_cmd = SUB(*cmd, 0, grp->curs_pos - START_POS);
		i = -1;
		while (order[++i] != '\0') // print ici
			beg_cmd = ft_charjoin(beg_cmd, order[i]);
		end_cmd = SUB(*cmd, grp->curs_pos - START_POS, LEN(*cmd));
		ft_putstr_fd(end_cmd, 2);
		ft_tputs(NULL, "ch");
		*cmd = JOIN(beg_cmd, end_cmd);
	}
	else
	{
		while (order[++i] != '\0')
			*cmd = ft_charjoin(*cmd, order[i]);
		//*cmd = ft_charjoin(*cmd, key);
	}
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

int		key_selection(t_group *grp, char *order, char **cmd)
{
	int	key;

	key = KEY(order[0], order[1], order[2], order[3]);
	if (key == CTRL_D || key == ESC)
	{
		reset_shell();
		ft_putchar('\n');
		exit(0);
	}
	if (grp && key == ENTER)
		return (ENTER);
	else if (key == BACKSPACE)
		handling_backspace(grp, cmd);
	else if (key == DEL)
		remove_line(grp, cmd);
	else if (key == ARROW_L || key == ARROW_R || key == ARROW_U || key == ARROW_D)
		handling_arrow(grp, cmd, key);
	else /*if (ft_isprint(key) || key == CTRL_V)*/
		print_cmd(grp, key, order, cmd);
	//else
		//ft_putnbr(key);
	return (0);
}

void	read_cmd(t_group *grp, int fd, char **cmd)
{
	int		ret;
	char	order[BUF_SIZE + 1];

	ft_bzero(order, BUF_SIZE + 1);
	while ((ret = read(fd, order, BUF_SIZE)))
	{
		//printf("%d, %d, %d, %d\n",order[0], order[1], order[2], order[3]);
		order[ret] = '\0';
		if (key_selection(grp, order, cmd) == ENTER)
		{
			grp->curs_pos = START_POS;
			ft_putchar('\n');
			break ;
		}
		ft_bzero(order, BUF_SIZE + 1);
	}
}
