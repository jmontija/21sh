/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/13 22:24:17 by jmontija         ###   ########.fr       */
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

void	exec_cmd_redir(t_group *grp, char *cmd_to_exec)
{
	char	**exec_cmd;
	int		i;

	i = -1;
	//printf("COMMAND TO EXEC BY REDIR -> %s\n", cmd_to_exec);
	exec_cmd = ft_spacesplit(cmd_to_exec);
	while (exec_cmd[++i])
	{
		ft_putendl_fd(JOIN("COMMAND TO EXEC BY REDIRECTION -> " ,exec_cmd[i]), 2);
		exec_cmd[i] = ft_strtrim(exec_cmd[i]);
	}
	execve(search_exec(grp, exec_cmd[0]), exec_cmd, NULL) < 1 ? ft_putendl("error pipe execve") : 0;
	// env a placer a la place de NULL le stocker dans grp->env !
	exit(0);
}

void	create_redirection_to(t_group *grp, t_redir *curr)
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

/*
char	**create_redirection_from(char **cmd_line, int idx, int action)
{
	int	fd;

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
}
*/

void	create_redirection_from(void)
{
	printf("LAST REDIR FROM\n");
}

void	manage_redirection_from(t_group *grp, t_redir *curr)
{
	/*int			fd;
	pid_t		pid;
	int			buf;
	char		*cmd_to_exec;

	pid = fork();
	pid == -1 ? exit(270) : 0;
	cmd_to_exec = JOIN("cat ", curr->name);
	if (pid == 0)
	{
		fd = open("TESTFINAL", O_WRONLY | O_APPEND | O_CREAT, 0644);
		dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
		exec_cmd_redir(grp, cmd_to_exec);
		close(fd);
	}
	else if (pid != 0)
		waitpid(pid, &buf, 0);*/
}

int		exec_redir(int exec, t_group *grp, char *cmd)
{
	int		i;
	t_redir *curr;
	t_redir *trash;
	int		find_redir = false;

	i = -1;
	exec ? ft_putstr("EXEC REDIRECTION with ") : ft_putstr("FREE REDIRECTION for ");
	ft_putendl(cmd);
	while (++i < 3)
	{
		curr = grp->redirect[i];
		while (curr != NULL)
		{
			find_redir = true; // set a false la commande de base sera executer avec les > < etc ..
			if (exec == 1)
			{
				ft_putstr(curr->symbol); ft_putendl(curr->name);
				i == 0 ? manage_redirection_from(grp, curr) : create_redirection_to(grp, curr);
			}
			REMOVE(&curr->name);
			REMOVE(&curr->symbol);
			REMOVE(&curr->command);
			curr->action = false;
			trash = curr;
			curr = curr->next;
			ft_memdel((void *)trash);
		}
		i == 0 ? create_redirection_from() : 0;
		grp->redirect[i] = NULL;
	}
	return (find_redir ? 0 : -1);
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

int		insert_redir(t_group *grp, char *file, char *symbol)
{
	t_redir	*new;
	t_redir	*curr;
	char	*symbol_tmp;
	int		sym;

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
		ft_putstr("insert FIRST redirection -> "); ft_putendl(file);
		grp->redirect[sym] = new;
		return (1);
	}
	curr = grp->redirect[sym];
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
	ft_putstr("insert LAST redirection -> "); ft_putendl(file);
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
