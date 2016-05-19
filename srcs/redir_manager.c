/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 18:41:52 by julio             #+#    #+#             */
/*   Updated: 2016/05/19 19:52:07 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*last_pars_redir(char *new_cmd, t_redir *curr)
{
	int		i;
	char **file;

	i = 0;
	file = ft_spacesplit(curr->name);
	curr->name = file[0];
	if (file[1] != NULL)
	{
		while (file[++i])
		{
			new_cmd = JOIN(new_cmd, " ");
			new_cmd = JOIN(new_cmd, file[i]);
		}
	}
	printf("NEW CMD WITH ARG %s\n", new_cmd);
	return (new_cmd);
}

int		insert_redir(t_group *grp, char *file, char *symbol)
{
	t_redir	*new;
	t_redir	*curr;
	char	*symbol_tmp;
	int		sym;

	symbol_tmp = ft_charjoin("", *symbol);
	printf("sym = %s\n", symbol_tmp);
	sym = ft_atoi(ft_findocc(true, symbol_tmp, "< >")); // rajouter << si on doit les executr avnt les < a check !
	printf("sym = %d\n", sym);
	new = (t_redir *)malloc(sizeof(t_redir));
	new->name = SDUP(file);
	new->action = ft_strcmp(symbol, ">") == 0 ? O_TRUNC : O_APPEND; /* faire un systeme pour savoir que cest un close fd */
	new->symbol = symbol;
	grp->curr_cmd = last_pars_redir(grp->curr_cmd, new);
	new->command = SDUP(grp->curr_cmd);
	new->next = NULL;
	if (grp->redirect[sym] == NULL)
	{
		ft_putstr("insert FIRST redirection -> "); ft_putendl(file);
		grp->redirect[sym] = new;
		return (1);
	}
	curr = grp->redirect[sym];
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	ft_putstr("insert LAST redirection -> "); ft_putendl(file);
	return (1);
}

int		count_redirection(char *curr_pipe_cmd, char *symbol)
{
	int symlen;
	int	cnt;
	int	i;

	i = 0;
	cnt = 0;
	symlen = ft_strlen(symbol);
	while (curr_pipe_cmd && curr_pipe_cmd[i] != '\0')
	{
		if ( (symlen > 1 && strncmp(curr_pipe_cmd + i, symbol, symlen) == 0) ||
			(symlen == 1 && *symbol == curr_pipe_cmd[i] /*&& curr_pipe_cmd[i + 1] != '&' &&
				curr_pipe_cmd[i + 1] != *symbol && curr_pipe_cmd[i - 1] != *symbol*/) )
		{
			cnt++;
			i += symlen;
		}
		else
			i++;
	}
	printf("IN REDIRECTION with '%dx %s'\nCURR_PIPE_CMD = %s\n", cnt, symbol, curr_pipe_cmd);
	return (cnt);
}

int		main_redirection(t_group *grp, char **split_cmd, char *symbol)
{
	int		i;
	int		cnt_redir;
	char	*cmd;

	i = -1;

	if (grp->curr_pipe_cmd)
		cnt_redir = count_redirection(grp->curr_pipe_cmd, symbol);
	else
		cnt_redir = count_redirection(grp->order, symbol);
	while (split_cmd[++i])
	{
		ft_putendl(split_cmd[i]);
		grp->curr_pipe_cmd = SDUP(split_cmd[i]); // ATTENTIOn
		if (split_cmd[i][0] == '>' || split_cmd[i][0] == '<')
			error_synthax("error parsing near", symbol);
		if (i > 0)
		{
			cmd = get_cmd(grp, split_cmd[i]);
			insert_redir(grp, cmd, symbol);
		}
		ft_parsing(1, split_cmd[i]);
	}
	if (i < cnt_redir + 1)
		error_synthax("error parsing near", symbol);
	return (1);
}