/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_lib_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/19 00:56:23 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/23 00:08:35 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_group	*nothing(t_group *grp)
{
	return (grp);
}

int		check_file(t_group *grp, char *name, int rights)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(name, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	nothing(grp);
	if (ret != 0)
		error_cmd("unknown file", name);
	else if (!(val & rights))
		error_cmd("Permission denied", name);
	else if (!S_ISREG(s_buf.st_mode))
		error_cmd("file required", name);
	else
		return (0);
	return (-1);
}

char	*last_pars_file(t_group *grp, char *new_cmd, char *curr_file,
	t_redir *new)
{
	int			i;
	char		**file;

	file = ft_spacesplit(curr_file);
	if (file[0] == '\0')
	{
		error_synthax("error parsing near", new->symbol);
		return (NULL);
	}
	take_off_quotes(&file[0]);
	new->name = SDUP(file[0]);
	if (ft_strcmp(new->symbol, "<") == 0 &&
		check_file(grp, new->name, S_IRUSR) < 0)
		return (NULL);
	i = 0;
	while (file[++i] != NULL)
	{
		new_cmd = JOIN(new_cmd, " ");
		new_cmd = JOIN(new_cmd, file[i]);
	}
	return (new_cmd);
}

int		insert_fd(int idx_cmd, t_group *grp, char *file, char *symbol)
{
	t_redir	*new;
	t_redir	*curr;

	new = (t_redir *)malloc(sizeof(t_redir));
	if (!ft_strcmp(symbol, ">") || !ft_strcmp(symbol, ">>") ||
		!ft_strcmp(symbol, "<") || !ft_strcmp(symbol, "<<") ||
		!ft_strcmp(symbol, ">&") || !ft_strcmp(symbol, ">>&"))
		new->symbol = SDUP(symbol);
	else
		return (error_synthax("unavailable symbol", symbol));
	if ((grp->curr_cmd = last_pars_file(grp, grp->curr_cmd, file, new)) == NULL)
		return (-1);
	new->cmd_split = NULL;
	new->next = NULL;
	if (grp->sh_cmd[idx_cmd] == NULL)
	{
		grp->sh_cmd[idx_cmd] = new;
		return (0);
	}
	curr = grp->sh_cmd[idx_cmd];
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	return (0);
}
