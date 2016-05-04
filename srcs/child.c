/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/23 14:17:42 by julio             #+#    #+#             */
/*   Updated: 2016/05/04 17:58:59 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	path_rights(char *path_env)
{
	struct stat	s_buf;
	mode_t		val;

	lstat(path_env, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (!(val & S_IXUSR))
	{
		ft_putstr("fsh: /!\\ WARNING PATH /!\\: permission denied: ");
		ft_putendl(path_env);
	}
}

char	*search_exec(t_group *grp, char *cmd)
{
	char	**path_env;
	char	*path_env_line;
	char	*path;
	char	*tmp;
	int		i;

	i = -1;
	path = NULL;
	if ((path_env_line = ft_getenv(grp, "PATH")) == NULL)
	{
		tmp = JOIN("env ", grp->order);
		grp->cmd = ft_spacesplit(tmp);
		path = ft_getenv(grp, "_");
		return (path);
	}
	path_env = ft_strsplit(path_env_line, ':');
	while (path_env[++i] != NULL)
	{
		REMOVE(&path);
		path = JOIN(path_env[i], cmd);
		path_rights(path_env[i]);
		if (access(path, F_OK) == 0)
			break ;
	}
	return (path);
}

char	*child_process(t_group *grp, char *order)
{
	int	i;

	i = 0;
	order = ft_strtrim(order);
	grp->order = SDUP(order);
	if (exec_builtin(grp, order) <= 0)
		return (search_exec(grp, grp->cmd[0]));
	return (NULL);
}
