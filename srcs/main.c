/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 18:38:24 by julio             #+#    #+#             */
/*   Updated: 2016/05/03 02:04:57 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_prompt(int signum)
{
	signum = 0;
	ft_putchar('\n');
	ft_putstr("> ");
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
	insert_env(grp, "SHLVL=1");
	i = -1;
	while (env[++i] != NULL)
		insert_env(grp, env[i]);
	insert_env(grp, "_=/usr/bin/env");
	return (1);
}

int		main(int argc, char **argv, char **env)
{
	t_group	*grp;
	(!argc || !argv) ? exit(0) : 0;
	grp = init_grp();
	init_shell();
	set_shell((~ICANON & ~ECHO));
	stock_env(grp, env) ? shlvl(grp) : 0;
	signal(SIGINT, ft_prompt);
	signal(SIGTSTP, ft_prompt);
	signal(SIGQUIT, ft_prompt);
	while (7)
		parse_cmd(0, grp);
	return (0);
}
