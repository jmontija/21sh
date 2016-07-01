/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_lib_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 18:43:09 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/23 00:08:23 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		check_cmd(char *path, char **cmd_line)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd("unknown command", cmd_line[0]);
	else if (s_buf.st_size <= 0)
		error_cmd("executable format error", cmd_line[0]);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("Permission denied", cmd_line[0]);
	else
		return (0);
	return (-1);
}

void	take_off_quotes(char **cmd)
{
	char	*cpy_cmd;
	int		i;

	i = -1;
	cpy_cmd = ft_strdup("");
	while ((*cmd)[++i])
	{
		if ((*cmd)[i] != '"' && (*cmd)[i] != '\'')
			cpy_cmd = ft_charjoin(cpy_cmd, (*cmd)[i]);
	}
	*cmd = SDUP(cpy_cmd);
	REMOVE(&cpy_cmd);
}

int		split_cmd(t_group *grp, t_redir *new)
{
	char	*path;
	int		i;

	i = -1;
	grp->cmd = ft_spacesplit(grp->curr_cmd);
	while (grp->cmd[++i] != NULL)
		grp->cmd[i] = ft_strtrim(grp->cmd[i]);
	take_off_quotes(&grp->cmd[0]);
	if (grp->cmd[0][0] != '.' && grp->cmd[0][0] != '/')
		path = child_process(grp, NULL);
	else
		path = SDUP(grp->cmd[0]);
	if (path != NULL)
	{
		if (check_cmd(path, grp->cmd) < 0)
			return (-1);
		new->fd = 1;
		new->name = SDUP(path);
	}
	else if (path == NULL)
	{
		new->fd = 0;
		new->name = SDUP(grp->curr_cmd);
	}
	return (0);
}

char	**insert_split_cmd(char ***cmd)
{
	int		i;
	char	**cpy_cmd;

	i = 0;
	while ((*cmd)[i] != NULL)
		i++;
	cpy_cmd = (char **)malloc(sizeof(char *) * i + 1);
	i = -1;
	while ((*cmd)[++i] != NULL)
	{
		cpy_cmd[i] = SDUP((*cmd)[i]);
		take_off_quotes(&cpy_cmd[i]);
		REMOVE(&(*cmd)[i]);
	}
	cpy_cmd[i] = NULL;
	*cmd = NULL;
	return (cpy_cmd);
}

int		insert_cmd(int idx_cmd, t_group *grp)
{
	t_redir	*new;
	t_redir	*curr;

	new = (t_redir *)malloc(sizeof(t_redir));
	if (split_cmd(grp, new) < 0)
		return (-1);
	new->cmd_split = insert_split_cmd(&grp->cmd);
	new->symbol = NULL;
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
