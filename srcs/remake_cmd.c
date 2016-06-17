/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/29 19:02:34 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/17 03:28:23 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		check_fill_cmd(int idx_cmd, t_group *grp, char *cmd)
{
	int		i;
	int		ret;
	char	*file;
	char	*symbol;
	int		synth;

	i = -1;
	file = ft_strnew(0);
	symbol = ft_strnew(0);
	grp->curr_cmd = ft_strnew(0);
	ret = 0;
	synth = false;
	while (cmd[++i] != '\0')
	{
		synth = check_parentheses(grp, cmd[i]);
		while (synth == false && (cmd[i] == '>' || cmd[i] == '<'))
		{
			symbol = ft_charjoin(symbol, cmd[i]);
			i++;
		}
		if (cmd[i] == '&')
		{
			symbol = ft_charjoin(symbol, cmd[i]);
			i++;
		}
		if (symbol[0] == '\0')
			grp->curr_cmd = ft_charjoin(grp->curr_cmd, cmd[i]);
		else
			file = ft_charjoin(file, cmd[i]);
		if (synth == false && file[0] != '\0' && (cmd[i + 1] == '\0' || cmd[i + 1] == '>' || cmd[i + 1] == '<'))
		{
			ret = insert_fd(idx_cmd, grp, file, symbol);
			REMOVE(&symbol); REMOVE(&file);
			if (ret < 0)
				return (ret);
			symbol = ft_strnew(0); file = ft_strnew(0);
		}
	}
	ret = insert_cmd(idx_cmd, grp);
	REMOVE(&grp->curr_cmd);
	if (symbol[0] != '\0')
		return (error_synthax("error parsing near", symbol));
	return (ret);
}

int		split_order(t_group *grp)
{
	char	**cmd_to_exec;
	int		i;
	int		ret;

	i = -1;
	ret = 0;
	cmd_to_exec = ft_strsplitquote(grp->order, '|');
	while (cmd_to_exec[++i] != NULL)
	{
		cmd_to_exec[i] = ft_strtrim(cmd_to_exec[i]);
		if ((ret = check_fill_cmd(i, grp, cmd_to_exec[i])) < 0)
			return (-1);
	}
	if (i < grp->pipe + 1)
		return (error_synthax("Invalid null commander near", "|"));
	return (ret);
}

int		is_parenthese_closed(t_group *grp, int synth)
{
	int		i;

	TERM(other_read) = true;
	while (synth)
	{
		TERM(cmd_line) = SDUP("");
		ft_putstr_fd("\033[1;31m", 2);
		ft_putstr_fd("incomplete> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
		read_cmd(grp, 0);
		if (grp->exit[1] == true)
			break ;
		grp->order = ft_charjoin(grp->order, '\n');
		grp->order = JOIN(grp->order, TERM(cmd_line));
		i = -1;
		while (TERM(cmd_line)[++i])
			synth = check_parentheses(grp, TERM(cmd_line)[i]);
		REMOVE(&TERM(cmd_line));
	}
	i = -1;
	while (grp->order[++i] != '\0')
	{
		synth = check_parentheses(grp, grp->order[i]);
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
	//printf("ORDER RECOMPOSED QUOTE = %s\n", grp->order);
}

int		create_fd_to(t_group *grp)
{
	int	i;
	int	action;
	t_redir	*curr;

	i = -1;
	while (grp->sh_cmd[++i] != NULL)
	{
		curr = grp->sh_cmd[i];
		while (curr != NULL)
		{
			if (curr->symbol && curr->symbol[0] == '>')
			{
				action = (ft_strcmp(curr->symbol, ">") == 0 ||  ft_strcmp(curr->symbol, ">&") == 0) ? O_TRUNC : O_APPEND;
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
	int	i;
	int	ambigous;
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
					return (error_synthax("Ambiguous output redirect, too many", "'>'"));
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
	int 	synth;
	char	*splitw;

	i = -1;
	synth = false;
	while (grp->order[++i] != '\0')
		if ((synth = check_parentheses(grp, grp->order[i])) < 0)
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