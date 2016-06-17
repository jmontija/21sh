/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 18:43:09 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/14 03:11:00 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		check_file(t_group *grp, char *name, int rights)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(name, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
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

char	*last_pars_file(t_group *grp, char *new_cmd, char *curr_file, t_redir *new)
{
	int			i;
	char		**file;
	struct stat	s_buf;

	file = ft_spacesplit(curr_file);
	if (file[0] == '\0')
	{
		error_synthax("error parsing near", new->symbol);
		return (NULL);
	}
	new->name = SDUP(file[0]);
	if (ft_strcmp(new->symbol, "<") == 0  && check_file(grp, new->name, S_IRUSR) < 0)
		return (NULL);
	i = 0;
	while (file[++i] != NULL)
	{
		new_cmd = JOIN(new_cmd, " ");
		new_cmd = JOIN(new_cmd, file[i]);
		//printf("part_cmd -> %s\n", file[i]);
	}
	return (new_cmd);
}

int		insert_fd(int idx_cmd, t_group *grp, char *file, char *symbol)
{
	t_redir	*new;
	t_redir	*curr;

	new = (t_redir *)malloc(sizeof(t_redir));
	//printf("REDIR SYM = %s\n", symbol);
	if (!ft_strcmp(symbol, ">") || !ft_strcmp(symbol, ">>") || !ft_strcmp(symbol, "<") || !ft_strcmp(symbol, "<<") || !ft_strcmp(symbol, ">&"))
		new->symbol = SDUP(symbol);
	else
		return (error_synthax("unavailable symbol", symbol));
	if ((grp->curr_cmd = last_pars_file(grp, grp->curr_cmd, file, new)) == NULL)
		return (-1);
	new->cmd_split = NULL;
	new->next = NULL;
	if (grp->sh_cmd[idx_cmd] == NULL)
	{
		//ft_putstr("insert redirection -> "); ft_putendl(new->name);
		grp->sh_cmd[idx_cmd] = new;
		return (0);
	}
	curr = grp->sh_cmd[idx_cmd];
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	//ft_putstr("insert redirection -> "); ft_putendl(new->name);
	return (0);
}

int		check_cmd(t_group *grp, char *path, char **cmd_line)
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

int		split_cmd(t_group *grp, t_redir *new)
{
	char	*path;
	int		i;

	i = -1;
	grp->cmd = ft_spacesplit(grp->curr_cmd);
	while (grp->cmd[++i] != NULL)
		grp->cmd[i] = ft_strtrim(grp->cmd[i]);
	if (grp->cmd[0][0] != '.' && grp->cmd[0][0] != '/')
		path = child_process(grp, NULL);
	else
		path = SDUP(grp->cmd[0]);
	if (path != NULL)
	{
		if (check_cmd(grp, path, grp->cmd) < 0)
			return (-1);
		new->fd = 1; // valid cmd 1
		new->name = SDUP(path);
	}
	else if (path == NULL)
	{
		new->fd = 0; // builtin 0
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
		if ((*cmd)[i][0] == '"' || (*cmd)[i][0] == '\'')
			cpy_cmd[i] = SUB((*cmd)[i], 1, LEN((*cmd)[i]) - 2);
		else
			cpy_cmd[i] = SDUP((*cmd)[i]);
		//printf("insert cmd splited %s\n", cpy_cmd[i]);
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

	//printf("cmd[%d]= %s\n", idx_cmd, grp->curr_cmd);
	if (grp->curr_cmd[0] == '\0')
		return (error_synthax("Invalid null command near", "|"));
	new = (t_redir *)malloc(sizeof(t_redir));
	if (split_cmd(grp, new) < 0)
		return (-1);
	new->cmd_split = insert_split_cmd(&grp->cmd);
	new->symbol = NULL;
	new->next = NULL;
	if (grp->sh_cmd[idx_cmd] == NULL)
	{
		grp->sh_cmd[idx_cmd] = new;
		//ft_putstr("insert BINARY -> "); ft_putendl(new->name);
		return (0);
	}
	curr = grp->sh_cmd[idx_cmd];
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	//ft_putstr("insert BINARY -> "); ft_putendl(new->name);
	return (0);
}