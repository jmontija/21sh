/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/05 03:52:12 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/22 22:38:36 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	error_cmd(char *what, char *who)
{
	t_group *grp;

	grp = init_grp();
	show_prompt(grp, "fsh: ", 6, "\033[1;31m");
	ft_putstr_fd(what, 2);
	ft_putstr_fd(" -> ", 2);
	ft_putendl_fd(who, 2);
}

int		error_synthax(char *error, char *file)
{
	t_group *grp;

	grp = init_grp();
	error_cmd(error, file);
	grp->fd_in_save = 0;
	grp->pipe = 0;
	unlink(TMP_FROM);
	return (-1);
}

int		error_opt(char opt, char *what)
{
	t_group *grp;

	grp = init_grp();
	show_prompt(grp, "fsh: env: ", 6, "\033[1;31m");
	ft_putchar_fd(opt, 2);
	ft_putendl_fd(what, 2);
	ft_putendl_fd(
"usage: env [-iv] [-P altpath][-u name] [name=value] [utility [argument]", 2);
	return (-1);
}

void	cderr_pwd(t_group *grp, char *path, struct stat s_buf)
{
	mode_t		val;
	char		buf[1024];
	char		*old_pwd;
	char		*pwd;
	char		*dir_name;

	dir_name = ft_strrchr(path, '/');
	dir_name[0] == '/' ? dir_name = dir_name + 1 : 0;
	val = (s_buf.st_mode & ~S_IFMT);
	if (access(path, F_OK) != 0)
		error_cmd("unknown directory", dir_name);
	else if (!S_ISDIR(s_buf.st_mode) && !S_ISLNK(s_buf.st_mode))
		error_cmd("this is not a directory", dir_name);
	else if (!(val & S_IXUSR))
		error_cmd("Permission denied", dir_name);
	else if (chdir(path) == 0)
	{
		old_pwd = JOIN("OLDPWD=", ft_getenv(grp, "PWD"));
		pwd = JOIN("PWD=", getcwd(buf, 1024));
		insert_env(grp, pwd);
		!old_pwd ? (old_pwd = JOIN("OLDPWD=", ft_getenv(grp, "PWD"))) : 0;
		insert_env(grp, old_pwd);
	}
	else
		error_cmd("what the fuck you are doing ?", dir_name);
}

int		list_to_tab(int stock, t_env *env, char ***env_tab)
{
	char	*tmp;
	int		pos;

	pos = 0;
	while (env != NULL)
	{
		if (stock)
		{
			tmp = JOIN(env->name, "=");
			(*env_tab)[pos] = JOIN(tmp, env->val);
		}
		pos++;
		env = env->next;
	}
	return (pos);
}
