/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 18:43:19 by julio             #+#    #+#             */
/*   Updated: 2016/05/27 17:40:42 by jmontija         ###   ########.fr       */
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
				(symlen == 1 && *symbol[j] == cmd[i] && cmd[i + 1] != '&'/*&& cmd[i - 1] && cmd[i + 1] &&
					cmd[i + 1] != *symbol[j] && cmd[i - 1] != *symbol[j]*/) )
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

void	split_exec_cmd(t_group *grp, char *cmd_to_exec, char *toprint)
{
	char	**exec_cmd;
	char	*path;
	int		i;

	i = -1;
	exec_cmd = ft_spacesplit(cmd_to_exec);
	while (exec_cmd[++i])
	{
		ft_putendl_fd(JOIN(toprint, exec_cmd[i]), 2); // penser a supprimer les guillemets etc ...
		exec_cmd[i] = ft_strtrim(exec_cmd[i]);
	}
	grp->cmd = exec_cmd;
	if (grp->cmd[0][0] != '.' && grp->cmd[0][0] != '/')
		path = search_exec(grp, grp->cmd[0]); //child_process(grp, grp->cmd[0]);
	else
		path = SDUP(grp->cmd[0]);
	if (path != NULL)
		execve(path, grp->cmd, grp->env) < 1 ? ft_putendl("execve failed") : 0; //ATTTENTION LENV a ete copié par adresse peut creer des bug !
	exit(0);
}