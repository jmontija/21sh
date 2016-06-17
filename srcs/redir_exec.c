/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/06/17 03:36:37 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		ft_fdcopy(int fd_r, char *fromfd, int fd_w, char *tofd)
{
	char	*order;

	order = ft_strdup("");
	fd_r == 0 ? (fd_r = open(fromfd, O_RDONLY)) : 0;
	fd_w == 0 ? (fd_w = open(tofd, O_WRONLY | O_APPEND | O_CREAT, 0644)) : 0;
	while (get_next_line(fd_r, &order) > 0)
	{
		ft_putendl_fd(order, fd_w);
		REMOVE(&order);
	}
	return (true);
}

int		heredoc(t_group *grp, t_redir *curr)
{
	int	fd;

	TERM(other_read) = true;
	set_shell((~ICANON & ~ECHO));
	fd = open(TMP_FROM, O_WRONLY | O_APPEND | O_CREAT, 0644);
	while (grp->exit[1] == false)
	{
		TERM(cmd_line) = ft_strdup("");
		ft_putstr_fd("\033[1;34m", 2);
		ft_putstr_fd("heredoc> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
		read_cmd(grp, 0);
		if (ft_strcmp(TERM(cmd_line), curr->name) == 0)
			break ;
		ft_putendl_fd(TERM(cmd_line), fd);
		REMOVE(&TERM(cmd_line));
	}
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	REMOVE(&TERM(cmd_line));
	TERM(other_read) = false;
	reset_shell();
	return (true);
}

void	redir_from(int idx_cmd, t_group *grp)
{
	t_redir *curr;
	int		exec;
	int		exec_2;
	int		fd;

	exec = false;
	curr = grp->sh_cmd[idx_cmd];
	while (curr != NULL)
	{
		/*if (grp->fd_in_save > 0 && exec == false)
			exec = ft_fdcopy(grp->fd_in_save, NULL, 0, TMP_FROM);*/
		if (curr->symbol && ft_strcmp(curr->symbol, "<") == 0)
		{
			if (grp->fd_in_save > 1)
				exec_2 = ft_fdcopy(0, curr->name, grp->fd_in_save, NULL);
			else
				exec = ft_fdcopy(0, curr->name, 0, TMP_FROM);
		}
		else if (curr->symbol && ft_strcmp(curr->symbol, "<<") == 0)
			exec = heredoc(grp, curr);
		curr = curr->next;
	}
	if (exec)
	{
		fd = open(TMP_FROM, O_RDONLY);
		grp->fd_in_save = fd;
	}
	else if (exec_2)
	{
		fd = open("/dev/tty1", O_RDONLY);
		grp->fd_in_save = fd;
	} 
	//else if (exec)
		//ft_putendl("error fd failed");
}

int		redir_to(int idx_cmd, t_group *grp)
{
	t_redir *curr;
	int		exec;
	int		redir_all;

	curr = grp->sh_cmd[idx_cmd];
	exec = false;
	while (curr != NULL)
	{
		if (curr->symbol && curr->symbol[0] == '>')
		{
			redir_all = (ft_strcmp(curr->symbol, ">&") == 0) ? 1 : 0;
			exec = exec_command(curr->fd, grp, redir_all);
		}
		curr = curr->next;
	}
	return (exec);
}
