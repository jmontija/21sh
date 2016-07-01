/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   father.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 23:53:16 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/22 23:43:14 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	**get_command(t_group *grp, int const fd)
{
	char	order[512];
	char	**split;

	TERM(cmd_line) = SDUP("");
	ft_bzero(order, 513);
	fcntl(fd, F_GETPATH, order);
	if (ft_strcmp(order, "/dev/ttys000") == 0 ||
		ft_strcmp(order, "/dev/ttys001") == 0)
		show_prompt(grp, "fsh-> ", 6, "\033[1;32m");
	read_cmd(grp, fd);
	split = ft_strsplitquote(TERM(cmd_line), ';');
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

void	free_sh_cmd(t_group *grp)
{
	int		i;
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
}

void	lets_free(t_group *grp)
{
	check_parentheses(0);
	REMOVE(&grp->order);
	REMOVE(&grp->curr_cmd);
	grp->pipe = 0;
	grp->fd_in_save = STDIN_FILENO;
	grp->exit[1] = false;
	free_sh_cmd(grp);
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	REMOVE(&TERM(cmd_line));
	unlink(TMP_FROM);
}

void	parse_cmd(int const fd, t_group *grp)
{
	char	**all_cmd;
	int		i;

	i = -1;
	all_cmd = get_command(grp, fd);
	while (all_cmd[++i])
	{
		all_cmd[i] = ft_strtrim(all_cmd[i]);
		all_cmd[i] = check_shortcut(grp, all_cmd[i]);
		if (all_cmd[i][0] != '\0')
		{
			grp->order = SDUP(all_cmd[i]);
			insert_hist(grp, grp->order);
			if (check_synth_cmd(grp) >= 0)
			{
				reset_shell();
				main_pipe(grp, NULL);
				set_shell((~ICANON & ~ECHO));
			}
			lets_free(grp);
		}
	}
	grp->exit[0] ? exit_shell(grp, 0) : 0;
}
