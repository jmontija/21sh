/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/19 01:04:27 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/20 23:29:50 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	read_stock_symbol(t_group *grp, t_var *var, char *cmd)
{
	while (var->synth == false && (cmd[var->i] == '>' || cmd[var->i] == '<'))
	{
		var->symbol = ft_charjoin(var->symbol, cmd[var->i]);
		var->i += 1;
	}
	if (cmd[var->i] == '&')
	{
		var->symbol = ft_charjoin(var->symbol, cmd[var->i]);
		var->i += 1;
	}
	if (var->symbol[0] == '\0')
		grp->curr_cmd = ft_charjoin(grp->curr_cmd, cmd[var->i]);
	else
		var->file = ft_charjoin(var->file, cmd[var->i]);
}

int		insert_and_reset(int type, int idx_cmd, t_group *grp, t_var *var)
{
	int	ret;

	if (type == 1)
	{
		ret = insert_fd(idx_cmd, grp, var->file, var->symbol);
		REMOVE(&var->symbol);
		REMOVE(&var->file);
		if (ret < 0)
			return (ret);
		var->symbol = ft_strnew(0);
		var->file = ft_strnew(0);
	}
	else
	{
		if (grp->curr_cmd[0] == '\0')
			return (error_synthax("Invalid null command", "error"));
		ret = insert_cmd(idx_cmd, grp);
		REMOVE(&grp->curr_cmd);
		if ((var->symbol)[0] != '\0')
			return (error_synthax("error parsing near", var->symbol));
	}
	return (ret);
}

int		check_fill_cmd(int idx_cmd, t_group *grp, char *cmd)
{
	t_var	*var;

	var = (t_var *)malloc(sizeof(t_var));
	var->i = -1;
	var->synth = false;
	var->file = ft_strnew(0);
	var->symbol = ft_strnew(0);
	grp->curr_cmd = ft_strnew(0);
	while (cmd[++(var->i)] != '\0')
	{
		var->synth = check_parentheses(cmd[var->i]);
		read_stock_symbol(grp, var, cmd);
		if (var->synth == false && var->file[0] != '\0' &&
			(cmd[var->i + 1] == '\0' ||
				cmd[var->i + 1] == '>' || cmd[var->i + 1] == '<'))
			if (insert_and_reset(1, idx_cmd, grp, var) < 0)
				return (-1);
	}
	return (insert_and_reset(0, idx_cmd, grp, var));
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
		return (error_synthax("Invalid null commander near", cmd_to_exec[i]));
	return (ret);
}
