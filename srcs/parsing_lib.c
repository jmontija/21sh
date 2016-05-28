/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 18:43:19 by julio             #+#    #+#             */
/*   Updated: 2016/05/28 20:17:24 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*get_cmd(t_group *grp, char *cmd)
{
	char	**symbol;
	char	*shell_cmd;
	char	*tofind;
	size_t 	symlen;
	size_t		i;
	int			j;

	i = -1;
	tofind = SDUP("| 1>&2 2>&1 2>&- 1>&- >&- > >> < <<");
	symbol = ft_spacesplit(tofind);
	shell_cmd = SDUP(cmd);
	//printf("GET COMMAND %s\n", shell_cmd);
	while (cmd[++i] != '\0')
	{
		j = -1;
		while (symbol[++j] != NULL)
		{
			symlen = ft_strlen(symbol[j]);
			if ( (symlen > 1 && strncmp(cmd + i, symbol[j], symlen) == 0) ||
				(symlen == 1 && *symbol[j] == cmd[i] && cmd[i + 1] != '&') )
			{
				/* strsplitstr segftl si seulement le symbol est envoyée en 1ere pos de la cmd */
				shell_cmd = SDUP(ft_strsplitstr(cmd, symbol[j])[0]);
				return (shell_cmd);
			}
		}
	}
	return (shell_cmd);
}

int		ft_isdquote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

int		check_parenthese(char cmd, int synth)
{
	if (synth == 0 && ft_isdquote(cmd))
		synth = 1;
	else if (synth == 1 && ft_isdquote(cmd))
		synth = 0;
	return (synth);
}

void	error_synthax(char *error, char *file)
{
	t_group *grp;

	grp =init_grp();
	error_cmd(error, file);
	grp->fd_in_save = 0;
	unlink("./TESTFINAL");
	exit(0);
}

int		exec_cmd(t_group *grp, char *path, char **cmd_line)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd("unknown command", cmd_line[0]);
	else if (s_buf.st_size <= 0)
		error_cmd("executable format error", cmd_line[0]);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("Permission denied", cmd_line[0]);
	else
		return (1);
	return (-1);
}

void	split_exec_cmd(t_group *grp, char *cmd_to_exec, char *toprint)
{
	char	**exec;
	char	*path;
	int		i;

	i = -1;
	exec = ft_spacesplit(cmd_to_exec);
	while (exec[++i])
	{
		ft_putendl_fd(JOIN(toprint, exec[i]), 2); // penser a supprimer les guillemets etc ...
		exec[i] = ft_strtrim(exec[i]);
	}
	grp->cmd = exec;
	if (grp->cmd[0][0] != '.' && grp->cmd[0][0] != '/')
		path = child_process(grp, NULL);
	else
		path = SDUP(grp->cmd[0]);
	printf("CMD = %s FD_IN = %d\n", grp->cmd[0], grp->fd_in_save);
	if (path != NULL && exec_cmd(grp, path, grp->cmd) > 0)
		execve(path, grp->cmd, grp->env) < 1 ? ft_putendl_fd("execve failed", 2) : 0; //ATTTENTION LENV a ete copié par adresse peut creer des bug !
	else
		exec_builtin(1, grp, NULL); // a douille
	exit(0);
}