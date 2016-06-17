/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 18:38:24 by julio             #+#    #+#             */
/*   Updated: 2016/06/17 02:07:05 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_prompt(int signum)
{
	signum = 0;
	ft_putstr_fd("\033[1;32m", 2);
	ft_putstr_fd("\nfsh-> ", 2);
	ft_putstr_fd("\033[1;37m", 2);
}

char	*ft_getenv(t_group *grp, char *tofind)
{
	t_env	*env;

	env = grp->first;
	while (env != NULL)
	{
		if (ft_strcmp(env->name, tofind) == 0)
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

void	shlvl(t_group *grp)
{
	char	*shlvl;
	int		lvl;

	lvl = ft_atoi(ft_getenv(grp, "SHLVL"));
	lvl++;
	shlvl = ft_itoa(lvl);
	shlvl = JOIN("SHLVL=", shlvl);
	insert_env(grp, shlvl);
}

int		stock_env(t_group *grp, char **env)
{
	int		i;
	char	*pwd;
	char	*old_pwd;
	char	buf[512];

	pwd = JOIN("PWD=", getcwd(buf, 512));
	insert_env(grp, pwd);
	old_pwd = JOIN("OLDPWD=", ft_getenv(grp, "PWD"));
	insert_env(grp, old_pwd);
	insert_env(grp, "PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/local/munki");
	insert_env(grp, "SHLVL=1");
	i = -1;
	while (env[++i] != NULL)
		insert_env(grp, env[i]);
	insert_env(grp, "_=/usr/bin/env");
	return (1);
}

void	handler_win(int sig)
{
	t_group	*grp;
	struct winsize	w;

	grp = init_grp();
	ioctl(0, TIOCGWINSZ, &w);
	TERM(window->width) = w.ws_col;
	TERM(window->heigth) = w.ws_row;
}

void	handler_ctrl_c(int sig)
{
	t_group	*grp;

	grp = init_grp();
	if (TERM(cmd_line) == NULL)
	{
		ft_putchar_fd('\n', 2);
		grp->exit[1] = true;
		return ;
	}
	ft_go_end(grp);
	ft_bzero(TERM(cmd_line), LEN(TERM(cmd_line)));
	TERM(curs_pos) = 0;
	TERM(cmd_size) = 0;
	if (TERM(other_read) == true)
	{
		TERM(other_read) = false;
		ioctl(0, TIOCSTI, "\n");
		grp->exit[1] = true;
	}
	else
	{
		ft_putstr_fd("\033[1;32m", 2);
		ft_putstr_fd("\nfsh-> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
	}
}

int		main(int argc, char **argv, char **env)
{
	t_group	*grp;
	struct stat	s_buf;

	(!argc || !argv) ? exit(0) : 0;
	grp = init_grp();
	init_shell();
	set_shell((~ICANON & ~ECHO));
	stock_env(grp, env) ? shlvl(grp) : 0;
	signal(SIGINT, handler_ctrl_c);
	signal(SIGTSTP, ft_prompt);
	signal(SIGQUIT, ft_prompt);
	signal(SIGWINCH, handler_win);
	unlink(TMP_FROM); //ft_putendl("TMP_FROM DELETED");
	while (7)
		parse_cmd(0, grp);
	return (0);
}
