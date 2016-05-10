/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/10 20:39:26 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
	<> file -> make file
	cmd (> < >> <<) file [file1 file2 ...]options_cmd
	traité de gauche a droite
		-> cmd < file2 <file1 > file4 < file3  >file5
		-> cmd get info from file2 file1 file3 and redirect it to file4 file5 etc .. till next_cmd
	cmd < file > file -> envois un fd vide (faire  un cat et kill process)
	attention si 2 fois le meme fichier est appelé au cours d'une meme commande pour des redirections,
	comportement indeterminé (peut prendre en compte la redirection ou non selon le symbol qui suit,
	, si la redirection est similaire, a approfondir)
	exemple test :
		ls | grep -i ZKJSj < dkld | wc > TEST | grep -i '0' | wc -l > TEST | pwd ; cat TEST
			-> comportement indeterminé (spamer la cmd) !
		ls | grep -i ZKJSj < dkld | wc > TEST | grep -i 0 | wc -l > TEST ; cat TEST
		ls | grep -i ZKJSj < dkld | wc > TEST | grep -i 0 | wc -l ; cat TEST
*/

/*
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

	pour "<<" le pipe ne prend pas en compte la sortie eof, << et < redirige seulment la sortie
		exple_test: ls | grep -i OK <(<) FILE | grep -i "word_in_FILE" | wc -l
	/!\ Attention: modifie l'entrée de sa cmd (ici grep) et modifie donc sa sortie


	fd = open(cmd_line[idx + 2], O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (cmd_line);
}
*/

/*void	create_redirection_to(t_group *grp, int action)
{
	int			fd;
	pid_t		pid;
	int			buf;

	ft_putendl("create_redir_to");
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		fd = open(cmd_line[idx + 2], O_WRONLY | action | O_CREAT, 0644);
		dup2(grp->fd_save, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
		close(fd);
	}
	else if (pid != 0)
		waitpid(pid, &buf, 0);
}*/

int		make_redir(t_group *grp)
{
	t_redir *curr;

	if (grp->redirect == NULL)
		return(-1);
	ft_putendl("MAKE REDIRECTION with");
	curr = grp->redirect;
	while (curr != NULL)
	{
		ft_putstr(curr->symbol);
		ft_putendl(curr->name);
		curr = curr->next;
	}
	grp->redirect = NULL;
	return (0);
}

int		insert_redir(t_group *grp, char *cmd, char *symbol)
{
	t_redir	*new;
	t_redir	*curr;
	int		action;

	ft_putstr("insert redirection -> ");
	ft_putendl(cmd);
	action = ft_strcmp(symbol, ">") == 0 ? O_TRUNC : O_APPEND;
	new = (t_redir *)malloc(sizeof(t_redir));
	new->name = SDUP(cmd);
	new->action = action;
	new->symbol = symbol;
	new->next = NULL;
	if (grp->redirect == NULL)
	{
		grp->redirect = new;
		return (1);
	}
	curr = grp->redirect;
	while (curr->next != NULL)
	{
		curr = curr->next;
	}
	curr->next = new;
	return (1);
}

int		main_redirection(t_group *grp, char **split_cmd, char *symbol)
{
	int	i;
	static char *redir_to_cmd[2];

	i = -1;
	printf("IN REDIRECTION with %s\n", symbol);
	while (split_cmd[++i])
	{
		ft_putendl(split_cmd[i]);
		ft_parsing(1, split_cmd[i]);
		if (i > 0 && symbol[0] == '>')
			insert_redir(grp, ft_strsplit(split_cmd[i], '<')[0], symbol);
		else if (i > 0 && symbol[0] == '<')
			insert_redir(grp, split_cmd[i], symbol);
	}
/*
	idée tout stocké dans une liste (fd, sign, name)
	creer function qui boucle sur liste et exectuter make_redirection() avant un create_pipe
	ou si il ny a pas de pipe avant lexecv de base
*/
	return (1);
}
