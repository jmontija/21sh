/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/29 19:02:34 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/01 21:42:24 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	**cmd_remake(t_group *grp, char *cmd_to_rmk, char *symbol)
{
	char		**cmd_line;
	char		*tmp;
	int			i;

	printf("cmd after %s = %s\n", symbol , cmd_to_rmk);
	cmd_line = ft_spacesplit(cmd_to_rmk);
	if (cmd_line[0] && cmd_line[1])
	{
		printf("HERE %s %s\n", cmd_line[0], cmd_line[1]);
		tmp = cmd_line[0];
		cmd_line[0] = cmd_line[1];
		cmd_line[1] = tmp;
		i = 1;
		while (cmd_line[++i])
		{
			cmd_line[1] = ft_charjoin(cmd_line[1], ' ');
			cmd_line[1] = JOIN(cmd_line[1], cmd_line[i]);
		}
		return (cmd_line);
	}
	/*if (cmd_line[0] && cmd_line[1])
	{
		rmk = JOIN(cmd_line[1], symbol);
		rmk = JOIN(rmk, cmd_line[0]);
		i = 1;
		while (cmd_line[++i])
			rmk = JOIN(rmk, cmd_line[i]);
		printf("cmd remade %s\n", rmk);
		return (rmk);
	}*/
	return (NULL);
}

int		cmd_checker(t_group *grp, char *cmd_to_check, char *error_name)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;
	char		**cmd_line;
	char		*path;
	int			i;

	i = -1;
	cmd_line = ft_spacesplit(cmd_to_check);
	while (cmd_line[++i])
		cmd_line[i] = ft_strtrim(cmd_line[i]);
	grp->cmd = cmd_line;
	if (cmd_line[0][0] != '.' && cmd_line[0][0] != '/' && error_name[0] == 'u')
		path = child_process(grp, NULL);
	else
		path = SDUP(cmd_line[0]);
	if (path == NULL)
		return (0);
	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd(error_name, cmd_line[0]);
	/*else if (s_buf.st_size <= 0)
		error_cmd("executable format error", cmd_line[0]);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("Permission denied", cmd_line[0]);*/
	else
		return (0);
	return (-1);
}

int		check_synth_redir(t_group *grp, char **split_cmd, char *symbol)
{
	char	*splitw;
	char	*file;
	int		ret;
	int		i;

	i = -1;
	ret = 0;
	if (split_cmd[0][0] == '\0')
	{
		if ((split_cmd = cmd_remake(grp, split_cmd[1], symbol)) == NULL)
			return (error_synthax("error parsing near", symbol));
	}
	while (split_cmd[++i])
	{
		printf("split_cmd %s\n", split_cmd[i]);
		if (i > 0)
		{
			if (split_cmd[i][0] == '\0')
			{
				ret = error_synthax("error parsing near", symbol);
				break ;
			}
			file = get_cmd(grp, split_cmd[i]);
			if (symbol[0] == '>')
				printf("FILE REDIR TO %s\n", file); // a stocker dans une liste et creer a la fin si tout cest bien passé !
			else
			{
				if ((ret = cmd_checker(grp, file, "no such file or directory")) < 0)
					break ;
				printf("FILE REDIR FROM %s\n", file); // verifier ici si le fichier existe !
			}
		}
		if ((splitw = ft_findocc(false, split_cmd[i], ">> > << <")) != NULL)
		{
			ret = check_synth_redir(grp, ft_strsplitstr(split_cmd[i], splitw), splitw);
			if (ret < 0)
				break ;
		}
	}
	//printf("ORDER RECOMPOSED QUOTE = %s\n", grp->order);
	return (ret);
}

int		check_synth_pipe(t_group *grp, char **split_cmd)
{
	int			i;
	int			ret;
	char		*splitw;

	i = -1;
	/*REMOVE(&grp->order);
	grp->order = ft_strdup("");*/
	while (split_cmd[++i])
	{
		printf("CHECKER PIPE with CMD = %s\n", split_cmd[i]);
		if (split_cmd[i][0] == '\0')
		{
			ret = error_synthax("Invalid null command near", "|");
			break ;
		}
		if ((splitw = ft_findocc(false, split_cmd[i], ">> > << <")) != NULL)
		{
			ret = check_synth_redir(grp, ft_strsplitstr(split_cmd[i], splitw), splitw);
			if (ret < 0)
				break ;
		}
		if ((ret = cmd_checker(grp, split_cmd[i], "unknown command pipe")) < 0)
			break ;
		/*grp->order = JOIN(grp->order, split_cmd[i]);
		grp->pipe != i ? grp->order = ft_charjoin(grp->order, '|') : 0;*/
	}
	printf("ORDER RECOMPOSED PIPE = %s\n", grp->order);
	return (ret);
}

void	is_parenthese_closed(t_group *grp, int synth)
{
	char	*order;
	int		i;

	while (synth)
	{
		order = SDUP("");
		ft_putstr_fd("\033[1;34m", 2);
		ft_putstr_fd("quote> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
		read_cmd(grp, 0, &order);
		grp->order = ft_charjoin(grp->order, '\n');
		grp->order = JOIN(grp->order, order);
		i = -1;
		while (order[++i])
			synth = check_parenthese(order[i], synth);
		REMOVE(&order);
	}
	i = -1;
	while (grp->order[++i] != '\0')
	{
		synth = check_parenthese(grp->order[i], synth);
		if (synth == false && grp->order[i] == '|')
			grp->pipe += 1;
	}
	printf("ORDER RECOMPOSED QUOTE = %s\n", grp->order);
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
		synth = check_parenthese(grp->order[i], synth);
	is_parenthese_closed(grp, synth);
	if ((splitw = ft_findocc(false, grp->order, "| >> > << < 1>&2 2>&1 2>&- 1>&- >&-")) == NULL)
		splitw = SDUP(">"); // envoyer grp->order vers un checker fou !
	if (splitw[0] == '|')
		return (check_synth_pipe(grp, ft_strsplitstr(grp->order, splitw)));
	ret = check_synth_redir(grp, ft_strsplitstr(grp->order, splitw), splitw);
	/*if (ret == 0)
		ret = cmd_checker(grp, get_cmd(grp, grp->order), "unknown command redir");*/ // <- to check cmd after evthg up to date > TEST ls
	return (ret);
}