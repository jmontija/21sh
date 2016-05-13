/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/13 16:07:16 by julio            ###   ########.fr       */
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
void	exec_cmd_redir(t_group *grp, char *cmd_to_exec)
{
	char	**exec_cmd;
	int		i;

	i = -1;
	exec_cmd = ft_spacesplit(cmd_to_exec);
	while (exec_cmd[++i])
			exec_cmd[i] = ft_strtrim(exec_cmd[i]);
	execve(search_exec(grp, exec_cmd[0]), exec_cmd, NULL) < 1 ? ft_putendl("error pipe execve") : 0;
	// env a placer a la place de NULL le stocker dans grp->env !
	exit(0);
}

void	create_redirection_to(t_group *grp, t_redir *curr, int action)
{
	int			fd;
	pid_t		pid;
	int			buf;

	ft_putendl("create_redir_to");
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		fd = open(curr->name, O_WRONLY | curr->action | O_CREAT, 0644);
		dup2(grp->fd_in_save, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
		exec_cmd_redir(grp, curr->command);
		close(fd);
	}
	else if (pid != 0)
		waitpid(pid, &buf, 0);
}

char	*last_pars_redir(char *new_cmd, t_redir *curr)
{
	int		i;
	char **file;

	i = 0;
	file = ft_spacesplit(curr->name);
	curr->name = file[0];
	if (file[1] != NULL)
	{
		while (file[++i])
		{
			new_cmd = JOIN(new_cmd, " ");
			new_cmd = JOIN(new_cmd, file[i]);
		}
	}
	printf("NEW CMD WITH ARG %s\n", new_cmd);
	return (new_cmd);
}

int		exec_redir(t_group *grp, char *cmd)
{
	t_redir *curr;
	char	*new_cmd;
	t_bool	tmp = false;
	int		i = -1;	


	/*if (grp->redirect == NULL)
		return(-1);*/
	ft_putstr("EXEC REDIRECTION with ");
	ft_putendl(cmd);
	while (++i < 5)
	{
		curr = grp->redirect[i];
		while (curr != NULL)
		{
			//if (ft_strcmp(cmd, ft_spacesplit(curr->command)[0]) == 0)
			//{
				//new_cmd = last_pars_redir(new_cmd, curr);
				ft_putstr(curr->symbol); ft_putendl(curr->name);
				/*if (ft_strcmp(">", curr->symbol) == 0)
					create_redirection_to(grp, curr, O_TRUNC);
				else if (ft_strcmp(">>", curr->symbol) == 0)
					create_redirection_to(grp, curr, O_APPEND);
				else if (ft_strcmp("<", curr->symbol) == 0)*/
				//tmp = true;
				/* supprimer après avoir traité les redirs*/
			//}
			curr = curr->next;
		}
	}
	//grp->redirect = NULL;
	return (tmp ? 0 : -1);
}

int		insert_redir(t_group *grp, char *file, char *symbol)
{
	t_redir	*new;
	t_redir	*curr;
	char	*symbol_tmp;
	int		sym;

	ft_putstr("insert redirection -> "); ft_putendl(file);
	symbol_tmp = ft_charjoin("", *symbol);
	sym = ft_atoi(ft_findocc(1, symbol_tmp, "< >")); // rajouter << si on doit les executr avnt les < a check !
	printf("sym = %d\n", sym);
	new = (t_redir *)malloc(sizeof(t_redir));
	new->name = SDUP(file);
	new->action = ft_strcmp(symbol, ">") == 0 ? O_TRUNC : O_APPEND;
	new->symbol = symbol;
	grp->curr_cmd = last_pars_redir(grp->curr_cmd, new);
	new->command = SDUP(grp->curr_cmd);
	new->next = NULL;
	if (grp->redirect[sym] == NULL)
	{
		grp->redirect[sym] = new;
		return (1);
	}
	curr = grp->redirect[sym];
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	return (1);
}

int		main_redirection(t_group *grp, char **split_cmd, char *symbol)
{
	int	i;
	char *cmd;

	i = -1;
	printf("IN REDIRECTION with %s\n", symbol);
	while (split_cmd[++i])
	{
		ft_putendl(split_cmd[i]);
		if (i > 0)
		{
			cmd = get_cmd(grp, split_cmd[i]);
			insert_redir(grp, cmd, symbol);
		}
		ft_parsing(1, split_cmd[i]);
	}
	return (1);
}
