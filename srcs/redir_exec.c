/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/19 17:46:01 by jmontija         ###   ########.fr       */
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

		pour "<<" le pipe ne prend pas en compte la sortie eof, << et < redirige seulment la sortie
		exple_test: ls | grep -i OK <(<) FILE | grep -i "word_in_FILE" | wc -l
	/!\ Attention: modifie l'entrée de sa cmd (ici grep) et modifie donc sa sortie

	ATTENTION ZSH CREER LES FICHIER POUR LES REDIR > AVANT LEXECUTION DE LA COMMANDE
	exple
		-> ls | wc -l > TEST
		result = sizeof(ls + TEST) ;

	redirection de sortie dans la ligne de commande !
		-> >& FILE: redirige tout les fd(STDERR, STDOUT) vers FILE
		-> 1>&-: ferme le fd 1
		-> 1>&2: redirige le STDOUT vers STDERR
		-> CMD &> FILE mets en background la commande recuperable avec fg -> equivalent a un ctrl+Z sur un prog en cour

*/

void	create_redirection_to(t_group *grp, t_redir *curr)
{
	int			fd;
	pid_t		pid;
	int			buf;

	ft_putstr("CREATE_REDIR_TO ");
	ft_putendl(curr->name);
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		fd = open(curr->name, O_WRONLY | curr->action | O_CREAT, 0644);
		dup2(grp->fd_in_save, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
		split_exec_cmd(grp, curr->command, "COMMAND TO EXEC BY REDIRECTION_TO -> ");
		close(fd);
	}
	else if (pid != 0)
		waitpid(pid, &buf, 0);
}

void	create_redirection_from(t_group *grp)
{
	int			fd;
	pid_t		pid;
	int			buf;

	ft_putendl("CREATE_REDIR_FROM TESTFINAL");
	fd = open("TESTFINAL", O_RDONLY);
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid == 0)
	{
		if (ft_findocc(true, grp->order, "| >> >") == NULL)
		{
			dup2(fd, STDIN_FILENO);
			split_exec_cmd(grp, grp->curr_cmd, "COMMAND TO EXEC BY REDIRECTION_FROM -> ");
		}
		close(fd);
		exit(0);
	}
	else if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		grp->fd_in_save = fd;
		unlink("./TESTFINAL");
	}
	//manage_redirection_from(grp, "rm ", "TESTFINAL");
}

void	manage_redirection_from(t_group *grp, char *cmd, char *arg)
{
	int			fd;
	pid_t		pid;
	int			buf;
	char		*cmd_to_exec;
	struct stat	s_buf;

	if (lstat(arg, &s_buf) < 0)
	{
		error_cmd("no such file or directory", arg);
		grp->fd_in_save = fd;
		unlink("./TESTFINAL");
		exit(0);
	}
	pid = fork();
	pid == -1 ? exit(270) : 0;
	cmd_to_exec = JOIN(cmd, arg);
	if (pid == 0)
	{
		if (ft_strcmp(cmd, "cat ") == 0)
		{
			fd = open("TESTFINAL", O_WRONLY | O_APPEND | O_CREAT, 0644);
			dup2(fd, STDOUT_FILENO); // penser a reset le shell si cat ou autre fichier utilsant l'entree standard
		}
		split_exec_cmd(grp, cmd_to_exec, "COMMAND TO EXEC -> ");
		close(fd);
	}
	else if (pid != 0)
		waitpid(pid, &buf, 0);
}

int		exec_redir(int exec, t_group *grp, char *cmd)
{
	int		i;
	t_redir *curr;
	t_redir *trash;
	int		ret;

	i = -1;
	exec ? ft_putstr("EXEC REDIRECTION with ") : ft_putstr("FREE REDIRECTION for ");
	ft_putendl(cmd);
	ret = -1;
	while (++i < 3)
	{
		curr = grp->redirect[i];
		while (curr != NULL)
		{
			if (exec == 1)
			{
				ret = 0;
				ft_putstr(curr->symbol); ft_putchar(' '); ft_putendl(curr->name);
				i == 0 ? manage_redirection_from(grp, "cat ",curr->name) : create_redirection_to(grp, curr);
			}
			//REMOVE(&curr->name);
			//REMOVE(&curr->symbol)
			//REMOVE(&curr->command);
			curr->action = false;
			trash = curr;
			curr = curr->next;
			ft_memdel((void *)trash);
		}
		if (i == 0 && ret == 0)
			create_redirection_from(grp);
		grp->redirect[i] = NULL;
	}
	return (ret);
}
