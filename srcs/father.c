/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   father.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 23:53:16 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/17 03:01:27 by julio            ###   ########.fr       */
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
	char	order[512];
	char	**split;
	int		ret;

	TERM(cmd_line) = SDUP("");
	ft_bzero(order, 513);
	//readlink(JOIN(FD_DIR, ft_itoa(fd)), order, 512);
	/*fcntl(fd, F_GETPATH, order);
	if (ft_strcmp(order, "/dev/ttys000") == 0 ||
		ft_strcmp(order, "/dev/ttys001") == 0)
	{*/
		ft_putstr_fd("\033[1;32m", 2);
		ft_putstr_fd("fsh-> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
	//}
	read_cmd(grp, fd);
	split = ft_strsplit(TERM(cmd_line), ';');
	REMOVE(&TERM(cmd_line));
	ft_bzero(order, 513);
	return (split);
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

void		free_sh_cmd(t_group *grp)
{
	int	i;
	t_redir	*curr;
	t_redir	*trash;

	i = -1;
	while (grp->sh_cmd && grp->sh_cmd[++i] != NULL)
	{
		curr = grp->sh_cmd[i];
		while (curr != NULL)
		{
			REMOVE(&curr->symbol);
			REMOVE(&curr->name);
			curr->cmd_split = NULL;
			trash = curr;
			ft_memdel((void *)trash);
			curr = curr->next;
		}
		grp->sh_cmd[i] = NULL;
	}
	//ft_putendl("grp->sh_cmd FREE");
}

void		lets_free(t_group *grp)
{
	// VERIFIER ET FERMER LES FD OUVERT

	check_parentheses(grp, 0);
	REMOVE(&grp->order);
	REMOVE(&grp->curr_cmd);
	grp->pipe = 0;
	grp->fd_in_save = STDIN_FILENO;
	//grp->fd_in_psave = STDIN_FILENO;
	grp->exit[1] = false;
	free_sh_cmd(grp);
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	REMOVE(&TERM(cmd_line));
	unlink(TMP_FROM);
}

// ./21sh < file | env ls=djk test=dkjk ./21sh | wc <<eof

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
		j = -1;
		all_cmd[i] = ft_strtrim(all_cmd[i]);
		all_cmd[i] = check_shortcut(grp, all_cmd[i]);
		if (all_cmd[i][0] != '\0')
		{
			grp->order = SDUP(all_cmd[i]);
			insert_hist(grp, grp->order);
			if (check_synth_cmd(grp) >= 0)
				main_pipe(grp, NULL);
			lets_free(grp);
		}
	}
	grp->exit[0] ? exit(0) : 0;
}
