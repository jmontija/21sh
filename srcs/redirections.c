/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/06 20:08:53 by jmontija         ###   ########.fr       */
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
	while (cmd_line[++idx])
		REMOVE(&cmd_line[idx]);
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
	while (cmd_line[++idx])
		REMOVE(&cmd_line[idx]);
	return (cmd_line);
}

int		redirections(char *path, char **cmd_line, char **env)
{
	int	occ;
	int i;
	int	match;

	i = -1;
	occ = -1;
	match = false;
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
	return (match);
}

/*
	idée pars : check first stuff -> next check  the contrary (exple : first stuff '|', next stuff after cmd need to be redir file while we get new pipe etc ... )
*/
