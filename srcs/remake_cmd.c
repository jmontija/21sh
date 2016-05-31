/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/29 19:02:34 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/31 20:34:34 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
			(symlen == 1 && *symbol == curr_pipe_cmd[i] && curr_pipe_cmd[i + 1] != '&') )
		{
			cnt++;
			i += symlen;
		}
		else
			i++;
	}
	//printf("IN REDIRECTION with '%dx %s'\nCURR_PIPE_CMD = %s\n", cnt, symbol, curr_pipe_cmd);
	return (cnt);
}

int		cmd_checker(t_group *grp, char *path, char **cmd_line)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd("unknown comMand", cmd_line[0]);
	else if (s_buf.st_size <= 0)
		error_cmd("executable format error", cmd_line[0]);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("Permission denied", cmd_line[0]);
	else
		return (1);
	return (-1);
}

int		check_synth_redir(t_group *grp, char **split_cmd, char *symbol)
{
	char	*splitw;
	char	*file;
	int		cnt_redir;
	int		ret;
	int		i;

	i = -1;
	ret = 0;
	cnt_redir = count_redirection(grp->order, symbol);
	while (split_cmd[++i])
	{
		if (split_cmd[i][0] == '>' || split_cmd[i][0] == '<')
		{
			ret = error_synthax("error parsing near", symbol); // verifier ici si la synthax est > file cmd ! remettre ensuite la synthaxe dans le bon ordre
			break ;
		}
		if (i > 0)
		{
			file = get_cmd(grp, split_cmd[i]);
			if (symbol[0] == '>')
				printf("FILE REDIR TO %s\n", file); // a stocker dans une liste et creer a la fin si tout cest bien passé !
			else
				printf("FILE REDIR FROM %s\n", file); // verifier ici si le fichier existe !
		}
		else
			printf("CHECKER REDIR with CMD = %s\n", split_cmd[i]); // verifier ici si la commande est valide !
		if ((splitw = ft_findocc(false, split_cmd[i], ">> > << <")) != NULL)
			ret = check_synth_redir(grp, ft_strsplitstr(split_cmd[i], splitw), splitw);
	}
	if (ret == 0 && i < cnt_redir + 1)
		ret = error_synthax("error parsing near", symbol);
	return (ret);
}

int		check_synth_pipe(t_group *grp, char **split_cmd)
{
	int			i;
	int			j;
	int			ret;
	char		*path;
	char		*splitw;
	char		**pipe_cmd;
	struct stat	s_buf;

	i = -1;
	while (split_cmd[++i])
	{
		if (split_cmd[i][0] == '>' || split_cmd[i][0] == '<')
		{
			ret = error_synthax("error parsing near", "|"); // verifier ici si la synthax est > file cmd ! remettre ensuite la synthaxe dans le bon ordre
			break ;
		}
		printf("CHECKER PIPE with CMD = %s\n", split_cmd[i]); // verifier ici si la commande est valide comme fait si dessous le mettre dans une funcition a part!
		pipe_cmd = ft_spacesplit(get_cmd(grp, split_cmd[i]));	// here
		j = -1;
		while (pipe_cmd[++j])
			pipe_cmd[j] = ft_strtrim(pipe_cmd[j]);
		grp->cmd = pipe_cmd;
		path = child_process(grp, NULL);
		if (path && lstat(path, &s_buf) < 0)
		{
			ret = error_synthax("unknown command", pipe_cmd[0]);
			break ;
		}														// to here
		if ((splitw = ft_findocc(false, split_cmd[i], ">> > << <")) != NULL)
			ret = check_synth_redir(grp, ft_strsplitstr(split_cmd[i], splitw), splitw);
		if (ret == 0 && i == grp->pipe)
			return (0);
	}
	if (ret == 0)
		ret = error_synthax("Invalid null command near", "|");
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
	printf("ORDER RECOMPOSED = %s\n", grp->order);
}

int		check_synth_cmd(t_group *grp)
{
	int		i;
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
	return (check_synth_redir(grp, ft_strsplitstr(grp->order, splitw), splitw));
}