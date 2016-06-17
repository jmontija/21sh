/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/05 03:07:07 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/16 21:21:23 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
touch test
cat > test
cat test >> test

cat < test | wc -l

derniere commande ne marche pas si creer dans mon shell sinon oui
donc possibilité que des test soit fermé et louverture se passe mal a tester
*/

void	reset_opt(t_group *grp)
{
	int	i;

	i = -1;
	while (++i < 4)
		grp->define_cmd[i] = false;
	i = -1;
	while (++i < 7)
	{
		grp->options->on[i] = false;
		REMOVE(&grp->options->params[i]);
	}
	ft_putendl_fd("\nenv OPTIONS REINITIALIZED", 2);
}

void	show_or_overwrite(t_group *grp, t_env *curr)
{
	t_bool	overwrite;
	char	*key;
	char	*value;
	int		i;
	int		j;

	i = -1;
	overwrite = false;
	if (grp->options->params[u] == NULL ||
		ft_strcmp(grp->options->params[u], curr->name) != 0)
	{
		ft_putstr(curr->name);
		ft_putchar('=');
		overwrite = false;
		while (++i < grp->define_cmd[namenv] &&
			grp->cmd[i + grp->define_cmd[e_opt] + 1])
		{
			j = ft_strintchr(grp->cmd[i + grp->define_cmd[e_opt] + 1], '=');
			key = SUB(grp->cmd[i + grp->define_cmd[e_opt] + 1], 0, j);
			value = STRCHR(grp->cmd[i + grp->define_cmd[e_opt] + 1], '=') + 1;
			if (ft_strcmp(key, curr->name) == 0)
				(overwrite = true) ? ft_putendl(value) : 0;
		}
		overwrite == false ? ft_putendl(curr->val) : 0;
	}
}

void	show_env(t_group *grp)
{
	t_env	*curr;
	char	*key;
	int		i;
	int		j;

	i = -1;
	curr = grp->first;
	ft_putendl("\n\t\t\tFSH_ENVIRONNEMENT\n");
	while (curr != NULL && grp->options->on[ii] == false)
	{
		show_or_overwrite(grp, curr);
		curr = curr->next;
	}
	while (++i < grp->define_cmd[namenv] &&
		grp->cmd[i + grp->define_cmd[e_opt] + 1])
	{
		j = ft_strintchr(grp->cmd[i + grp->define_cmd[e_opt] + 1], '=');
		key = SUB(grp->cmd[i + grp->define_cmd[e_opt] + 1], 0, j);
		if (grp->options->on[ii] || ft_getenv(grp, key) == NULL)
			ft_putendl(grp->cmd[i + grp->define_cmd[e_opt] + 1]);
	}
}

void	update_env(t_group *grp)
{
	int	i;
	int	pos;

	pos = 0;
	i = -1;
	while (grp->env && grp->env[++i] != NULL)
		REMOVE(&(grp->env[i]));
	free(grp->env);
	grp->env = NULL;
	i = -1;
	while (++i < grp->define_cmd[namenv] && CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1])
		pos += 1;
	grp->env = (char **)malloc(sizeof(char *) * (pos + 1));
	i = -1;
	while (++i < pos + 1)
		grp->env[i] = NULL;
	i = -1;
	while (++i < (grp->define_cmd[namenv] -1) && CMD(cmd_split)[i + grp->define_cmd[e_opt]])
		grp->env[i] = SDUP(CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1]);
}

void	create_process_env(t_group *grp, char *path, char **cmd_line)
{
	pid_t	pid;
	int		buf;
	int		fd;

	if (path == NULL)
		return ;
	pid = fork();
	pid == -1 ? exit(270) : 0;
	if (pid != 0)
	{
		waitpid(pid, &buf, 0);
		buf == SIGSEGV ? error_cmd("segmentation fault", cmd_line[0]) : 0;
	}
	else if (pid == 0)
	{
		grp->options->on[ii] ? update_env(grp) : 0;
		dup2(grp->fd_in_save, STDIN_FILENO);
		execve(path, cmd_line, grp->env) < 1 ? ft_putendl_fd("execve failed", 2) : 0;
		exit(0);
	}
}


void	opt_v(t_group *grp, char **cmd)
{
	int	i;

	i = -1;
	grp->options->on[ii] ? ft_putendl_fd("#env clearing -> environ", 2) : 0;
	ft_putstr_fd("#env executing -> ", 2);
	ft_putendl_fd(cmd[0], 2);
	while (cmd[++i] != NULL)
	{
		ft_putstr_fd("#env arg[", 2);
		ft_putnbr_fd(i, 2);
		ft_putstr_fd("] -> ", 2);
		ft_putendl_fd(cmd[i], 2);
	}
}

int		exec_cmd(t_group *grp, char *path, char *cmd_line)
{
	struct stat	s_buf;
	mode_t		val;
	int			ret;

	ret = lstat(path, &s_buf);
	val = (s_buf.st_mode & ~S_IFMT);
	if (ret != 0)
		error_cmd("env: unknown command", cmd_line);
	else if (s_buf.st_size <= 0)
		error_cmd("env: executable format error", cmd_line);
	else if (!(val & S_IXUSR) || S_ISDIR(s_buf.st_mode))
		error_cmd("env: Permission denied", cmd_line);
	else
		return (1);
	return (-1);
}

int		check_env_var(t_group *grp)
{
	char	**modif_exec;
	char 	*env;
	int		ret;
	int		i;
	int		j;

	i = -1;
	ret = 0;
	modif_exec = ft_strsplit("LANG LC_ALL LC_CTYPE LC_MESSAGES NLSPATH PATH", ' ');
	list_to_tab(1, grp->first, &(grp->env_save));
	while (++i < grp->define_cmd[namenv] && CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1])
	{
		j = -1;
		env = CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1];
		while (modif_exec[++j] != NULL)
		{
			if (ft_strncmp(env, modif_exec[j], LEN(modif_exec[j])) == 0)
			{
				ret = insert_env(grp, CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1]);
				break ;
			}
		}
	}
	list_to_tab(1, grp->first, &(grp->env));
	return (ret);
}

void	backup_env(t_group *grp)
{
	int	i;

	i = -1;
	while (grp->env_save[++i] != NULL)
		insert_env(grp, grp->env_save[i]);
}

char	*env_cmd(t_group *grp, int opt)
{
	char	*path;
	int		cmd_nb;
	int		i;

	i = check_env_var(grp);
	cmd_nb = (opt == P) ? (grp->define_cmd[utils] + 1) : grp->define_cmd[utils];
	grp->cmd = grp->cmd + cmd_nb;
	if (opt == P)
		grp->cmd[0] = SDUP(grp->options->params[P]);
	if (grp->cmd[0][0] == '.' || grp->cmd[0][0] == '/')
		path = SDUP(grp->cmd[0]);
	else
		path = search_exec(grp, grp->cmd[0]);
	if (i == 1)
		backup_env(grp);
	if (exec_cmd(grp, path, grp->cmd[0]) < 0)
		return (NULL);
	return (path);
}

void	exec_env(t_group *grp, int display)
{
	char	*path;
	int		bin;

	if (display)
		return (show_env(grp));
	bin = grp->define_cmd[utils];
	if (grp->options->on[P] == false && grp->options->on[u] == false)
	{
		path = env_cmd(grp, v);
		grp->options->on[v] == true ? opt_v(grp, grp->cmd) : 0;
		create_process_env(grp, path, grp->cmd);
	}
	else if (grp->options->on[P] == true)
	{
		path = env_cmd(grp, P);
		grp->options->on[v] == true ? opt_v(grp, grp->cmd) : 0;
		create_process_env(grp, path, grp->cmd);
	}
}
