/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/07 21:38:14 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
	<> file -> make file
	cmd (> < >> <<) file [file1 file2 ...]options_cmd
	traité de gauche a droite
		-> cmd < file2 <file1 > file4 < file3  >file5
		-> cmd get info from file2 file1 file3 and redirect it to file4 file5 etc .. till next_cmd
	cmd < file > file -> envois un fd vide (faire  un cat et kill process);
*/

char	**create_redirection_to(char **cmd_line, int idx, int action)
{
	int		fd;
	t_group	*grp;

	grp = init_grp();
	if (cmd_line[idx + 2] == NULL)
	{
		error_cmd("error pars near", cmd_line[idx + 1]);
		return (NULL);
	}
	fd = open(cmd_line[idx + 2], O_WRONLY | action | O_CREAT, 0644);
	dup2(grp->fd_save, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
	close(fd);
	return (cmd_line);
}

char	**create_redirection_from(char **cmd_line, int idx, int action)
{
	int	fd;

	if (cmd_line[idx + 2] == NULL)
	{
		error_cmd("error pars near", cmd_line[idx + 1]);
		return (NULL);
	}

	/*
	si plusieurs '<', redirigé chaque file dans un nouveau file avec '>>'
		exple :
			-> wc < TEST < TEST2 < TEST3
				->	tab = {TEST, TEST2, TEST3};
					fd = open("TESTFINAL", O_WRONLY | O_APPEND | O_CREAT, 0644);
					while (tab[++i])
					{
						buf = strdup(tab[i]);
						size = strlen(buf);
						write(fd, buf, size);
					}
				->	wc < TESTFINAL ; rm -rf TESTFINAL
	 */

	fd = open(cmd_line[idx + 2], O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (cmd_line);
}

int		main_redirection(t_group *grp, char **split_cmd, char *symbol)
{
	printf("IN REDIRECTION with %s\n", symbol);
	char	checker;
	int		i;

	i = -1;
	checker = ft_parsing(split_cmd[0]);
	if (checker == false)
	{}
	return (1);
}
