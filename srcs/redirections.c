/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/04 03:24:19 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
	<> file -> make file
	cmd (> < >> <<) file [file1 file2 ...]options_cmd
	traité de gauche a droite
		-> cmd < file2 <file1 > file4 < file3  >file5
		-> cmd get info from file2 file1 file3 and redirect it to file4 file5 etc .. till next_cmd
*/

/*
	charspec = function(mychar)
	{
		int idx
		char_str = "< > |"
		char_tab = strsplit(char_str)
		
		while (chartab)
				if (char_tab[str][0] == 'mychar')
					return(str);
		return (-1);
	}
	ft_teaser = function(grp, cmd_line)
	{
		while (cmd_line)
			while(str)
				if (idx = charspec(char) >= 0)
					break
		pointeursurfonction(idx, &fd);
		parse(fd, grp)
	}

*/
char	**create_redirection(char **cmd_line, int idx, int action)
{
	int	fd;

	if (cmd_line[idx + 2] == NULL)
	{
		error_cmd("error pars near", cmd_line[idx + 1]);
		return (NULL);
	}
	fd = open(cmd_line[idx + 2], O_WRONLY | action | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
	close(fd);
	while (cmd_line[++idx])
		REMOVE(&cmd_line[idx]);
	return (cmd_line);
}

char 	**redirections(char **cmd_line)
{
	int	occ;
	int i;

	i = -1;
	occ = -1;
	while (cmd_line[++i])
	{
		if ((occ = ft_strintchr(cmd_line[i], '>')) >= 0)
		{
			if (occ == 0 && cmd_line[i][occ + 1] == '>')
				return (create_redirection(cmd_line, i - 1, O_APPEND));
			else if (occ == 0)
				return (create_redirection(cmd_line, i - 1, O_TRUNC));
		}
	}
	return (cmd_line);
}