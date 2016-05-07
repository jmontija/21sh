/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 18:38:38 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/07 21:37:24 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
OLD_PARSING:
	while (cmd_line[++i])
	{
		if ((occ = ft_strintchr(cmd_line[i], '>')) >= 0)
		{
			if (occ == 0 && cmd_line[i][occ + 1] == '>')
				cmd_line = create_redirection_to(cmd_line, i - 1, O_APPEND);
			else if (occ == 0)
				cmd_line = create_redirection_to(cmd_line, i - 1, O_TRUNC);
			execve(path, cmd_line, env) < 1 ? ft_putendl("error pipe execve") : (match = true);
		}
		else if ((occ = ft_strintchr(cmd_line[i], '<')) >= 0)
		{
			if (occ == 0 && cmd_line[i][occ + 1] == '<')
			{} // heredoc prompt a approfondir;
			else if (occ == 0)
				cmd_line = create_redirection_from(cmd_line, i - 1, O_TRUNC);
			execve(path, cmd_line, env) < 1 ? ft_putendl("error pipe execve") : (match = true);
		}
		else if ((occ = ft_strintchr(cmd_line[i], '|')) >= 0)
		{
			create_pipe(path, cmd_line, env, i);
			match = true;
		}
	}

idée pars : check first stuff -> next check  the contrary (exple : first stuff '|', next stuff after cmd need to be redir file while we get new pipe etc ... )
*/

char	*ft_strchrsym(char *str, char *tofind)
{
	size_t	i;
	char	*tocmp;

	i = -1;
	while (str[++i] != '\0')
	{
		tocmp = ft_charjoin("", str[i]);
		if (str[i + 1] && str[i] == str[i + 1])
		{
			tocmp = ft_charjoin(tocmp, str[i + 1]);
			i++;
		}
		if (strcmp(tocmp, tofind) == 0)
			return (tofind);
	}
	return (NULL);
}

char	*ft_findocc(char *order, char *symbol)
{
	size_t	i;
	char	**tofind;

	i = -1;
	tofind = ft_spacesplit(symbol);
	while (tofind[++i] != NULL)
		if (ft_strchrsym(order, tofind[i]) != NULL)
			return (tofind[i]);
	return (NULL);
}

// ls | CMD0 > FILE | CMD1 | CMD2 > FILE2 < OK
// ls -i DIR > FILE | CMD1 | CMD2 > FILE2 < OK
// checker si cest pas mieux de split sur les pipe first

int		ft_parsing(char	*to_pars)
{
	t_group	*grp;
	int 	i;
	char	*splitw;
	char	**split_cmd;

	i = -1;
	grp = init_grp();
	if ((splitw = ft_findocc(to_pars, "> >> < << |")) == NULL)
		return (false);
	split_cmd = ft_strsplit(to_pars, splitw[0]);
	while (split_cmd[++i])
	{
		split_cmd[i] = ft_strtrim(split_cmd[i]);
		ft_putendl(split_cmd[i]);
	}
	splitw[0] != '|' ? main_redirection(grp, split_cmd, splitw) : main_pipe(grp, split_cmd);
	return (splitw[0]);
}
