/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:05:11 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/15 02:39:01 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		ft_is_printable(char *order)
{
	int	i;

	i = -1;
	while (order[++i] != '\0')
		if (ft_isprint(order[i]) == false)
			return (0);
	return (1);
}

int		key_selection(t_group *grp, char *order)
{
	int	ret;
	int	key;

	key = KEY(order[0], order[1], order[2], order[3]);
	//printf("%d\n", key);
	if (key == ENTER)
		return (key);
	if (key != ARROW_U && key != ARROW_D)
		grp->curr_hist = NULL;
	if (key == CTRL_D)
		handling_ctrl_d(grp);
	else if (key == DEL)
		remove_line(grp);
	else if (key == BACKSPACE)
		handling_backspace(grp);
	else if (key == CTRL_L)
		handling_clear_screen(grp);
	else if (key == PAGE_UP)
		ft_go_up(grp);
	else if (key == PAGE_DOWN)
		ft_go_down(grp);
	else if (key == HOME)
		ft_go_home(grp);
	else if (key == END)
		ft_go_end(grp);
	else if (key == ALT_L)
		ft_prev_word(grp);
	else if (key == ALT_R)
		ft_next_word(grp);
	else if (key == ARROW_L || key == ARROW_R || key == ARROW_U || key == ARROW_D)
		handling_arrow(grp, key);
	else if (ft_is_printable(order))
		print_cmd(grp, order);
	else 
		return (0);
	return (1);
}

void		read_fd_in(t_group *grp, char *order)
{
	int	i;

	i = -1;
	while (order && order[++i] != '\0')
	{
		if (order[i] != '\n')
			TERM(cmd_line) = ft_charjoin(TERM(cmd_line), order[i]);
		else if (order[i + 1] != '\0')
			TERM(cmd_line) = JOIN(TERM(cmd_line), " ; ");
	}
	ft_putstr_fd("\033[1;32m", 2);
	ft_putstr_fd("command_line -> ", 2);
	ft_putstr_fd("\033[1;37m", 2);
	ft_putstr_fd(TERM(cmd_line), 2);
	grp->exit[0] = true;
}

void	read_cmd(t_group *grp, int fd)
{
	int		ret;
	char	*tmp;
	char	order[BUF_SIZE + 1];

	tmp = NULL;
	ret = -1;
	ft_bzero(order, BUF_SIZE + 1);
	while ((ret = read(fd, order, BUF_SIZE)) > 0)
	{
		order[ret] = '\0';
		tmp = ft_strdup(order);
		if (key_selection(grp, order) == '\n')
			break ;
		ft_bzero(order, BUF_SIZE + 1);
	}
	if (ret == 0)
		read_fd_in(grp, tmp);
	ft_putchar_fd('\n', 2);
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	ft_bzero(order, BUF_SIZE + 1);
	REMOVE(&tmp);
}
