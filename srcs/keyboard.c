/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:05:11 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/01 19:50:30 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		manage_curs_col(t_group *grp)
{
	struct winsize	window;

	ioctl(0, TIOCGWINSZ, &window);
	/*if (grp->curs_col >= window.ws_col)
		grp->line += 1;
	return (grp->line == 0 ? START_POS + LEN(*cmd) : )*/

}

void	print_cmd(t_group *grp, int key, char *order, char **cmd)
{
	char	*beg_cmd;
	char	*end_cmd;
	size_t	i;

	i = -1;
	while (order[++i] != '\0')
		if (ft_isprint(order[i]) == false)
			return ;
	ft_putstr_fd(order, 2);
	if (grp->curs_col < START_POS + LEN(*cmd))
	{
		beg_cmd = SUB(*cmd, 0, grp->curs_col - START_POS);
		beg_cmd = JOIN(beg_cmd, order);
		end_cmd = SUB(*cmd, grp->curs_col - START_POS, LEN(*cmd));
		*cmd = JOIN(beg_cmd, end_cmd);
		ft_putstr_fd(end_cmd, 2);
		i = LEN(end_cmd);
		while (i-- > 0)
			ft_tputs(NULL, "le");
	}
	else
		*cmd = JOIN(*cmd, order);
	grp->curs_col += LEN(order);
}

void	remove_line(t_group *grp, char **cmd)
{
	size_t	i;

	i = -1;
	grp->curs_col = START_POS + LEN(*cmd) - 1;
	ft_tputs(NULL, "ch");
	while (++i < LEN(*cmd))
	{
		ft_tputs("le", NULL);
		ft_tputs("dc", NULL);
	}
	REMOVE(cmd);
	*cmd = SDUP("");
	grp->curs_col = START_POS;
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
	else
		print_cmd(grp, key, order, cmd);
	return (0);
}

void	read_cmd(t_group *grp, int fd, char **cmd)
{
	int		ret;
	char	order[BUF_SIZE + 1];

	ft_bzero(order, BUF_SIZE + 1);
	while ((ret = read(fd, order, BUF_SIZE)))
	{
		order[ret] = '\0';
		if (key_selection(grp, order, cmd) == ENTER)
		{
			grp->curs_col = START_POS;
			ft_putchar('\n');
			break ;
		}
		ft_bzero(order, BUF_SIZE + 1);
	}
}
