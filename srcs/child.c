/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/23 14:17:42 by julio             #+#    #+#             */
/*   Updated: 2016/06/19 00:52:03 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	path_rights(char *path_env)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path_env, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret == 0 && !(val & S_IXUSR))
	{
		ft_putstr_fd("fsh: /!\\ WARNING PATH /!\\: permission denied: ", 2);
		ft_putendl_fd(path_env, 2);
	}
}

char	*search_exec(t_group *grp, char *cmd)
{
	char	**path_env;
	char	*path_env_line;
	char	*path;
	int		i;

	i = -1;
	path = ft_strdup("");
	path_env_line = ft_getenv(grp, "PATH");
	path_env = ft_strsplit(path_env_line, ':');
	while (path_env && path_env[++i] != NULL)
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
	if (exec_builtin(0, grp, order) <= 0)
		return (search_exec(grp, grp->cmd[0]));
	return (NULL);
}
