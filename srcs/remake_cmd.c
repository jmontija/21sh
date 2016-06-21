/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/29 19:02:34 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/20 23:47:55 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		terminate(t_group *grp)
{
	int	i;
	int synth;

	i = -1;
	synth = false;
	while (grp->order[++i] != '\0')
	{
		synth = check_parentheses(grp->order[i]);
		if (synth == false && grp->order[i] == '|')
			grp->pipe += 1;
	}
	TERM(other_read) = false;
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	REMOVE(&TERM(cmd_line));
	if (grp->exit[1] == true)
	{
		grp->exit[1] = false;
		return (-1);
	}
	return (0);
}

int		is_parenthese_closed(t_group *grp, int synth)
{
	int		i;

	TERM(other_read) = true;
	while (synth)
	{
		TERM(cmd_line) = SDUP("");
		show_prompt(grp, "incomplete> ", 12, "\033[1;31m");
		read_cmd(grp, 0);
		if (grp->exit[1] == true)
			break ;
		grp->order = ft_charjoin(grp->order, '\n');
		grp->order = JOIN(grp->order, TERM(cmd_line));
		i = -1;
		while (TERM(cmd_line)[++i])
			synth = check_parentheses(TERM(cmd_line)[i]);
		REMOVE(&TERM(cmd_line));
	}
	return (terminate(grp));
}

int		create_fd_to(t_group *grp)
{
	int		i;
	int		action;
	t_redir	*curr;

	i = -1;
	while (grp->sh_cmd[++i] != NULL)
	{
		curr = grp->sh_cmd[i];
		while (curr != NULL)
		{
			if (curr->symbol && curr->symbol[0] == '>')
			{
				action = (ft_strcmp(curr->symbol, ">") == 0 ||
					ft_strcmp(curr->symbol, ">&") == 0) ? O_TRUNC : O_APPEND;
				curr->fd = open(curr->name, O_WRONLY | action | O_CREAT, 0644);
				if (check_file(grp, curr->name, S_IWUSR) < 0)
					return (-1);
			}
			curr = curr->next;
		}
	}
	return (0);
}

int		csh_check(t_group *grp)
{
	int		i;
	int		ambigous;
	t_redir	*curr;

	i = -1;
	while (grp->sh_cmd[++i] != NULL)
	{
		curr = grp->sh_cmd[i];
		ambigous = grp->sh_cmd[i + 1] != NULL ? true : false;
		while (curr != NULL)
		{
			if (curr->symbol && curr->symbol[0] == '>')
			{
				if (ambigous)
				{
					return (error_synthax(
						"Ambiguous output redirect, too many", "'>'"));
				}
				ambigous = true;
			}
			curr = curr->next;
		}
	}
	return (create_fd_to(grp));
}

int		check_synth_cmd(t_group *grp)
{
	int		i;
	int		ret;
	int		synth;
	char	*splitw;

	i = -1;
	synth = false;
	while (grp->order[++i] != '\0')
		if ((synth = check_parentheses(grp->order[i])) < 0)
			return (-1);
	if (is_parenthese_closed(grp, synth) < 0)
		return (-1);
	i = -1;
	grp->sh_cmd = (t_redir **)malloc(sizeof(t_redir *) * (grp->pipe + 2));
	while (++i < grp->pipe + 2)
		grp->sh_cmd[i] = NULL;
	ret = split_order(grp);
	ret = ret >= 0 ? csh_check(grp) : -1;
	return (ret);
}
