/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/03 19:23:59 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
	l'arg suivant la redirection est forcement un fichier
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
	dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou tout autre fichier utilsant l'entree standard
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