/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 18:38:24 by julio             #+#    #+#             */
/*   Updated: 2016/06/23 17:37:02 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
	REMOVE(&grp->tmp_from);
	grp->tmp_from = JOIN("/tmp/.tmp_from", shlvl);
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
	insert_env(grp,
"PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/local/munki");
	insert_env(grp, "SHLVL=2");
	i = -1;
	while (env[++i] != NULL)
		insert_env(grp, env[i]);
	insert_env(grp, "_=/usr/bin/env");
	return (1);
}

void	stock_hist(t_group *grp)
{
	char	*order;
	int		fd;

	if ((fd = open("/nfs/2015/j/jmontija/.fsh_history", O_RDONLY)) < 0)
		return ;
	order = NULL;
	while (get_next_line(fd, &order) > 0)
	{
		insert_hist(grp, order);
		REMOVE(&order);
	}
}

int		main(int argc, char **argv, char **env)
{
	t_group		*grp;

	(!argc || !argv) ? (exit(0)) : 0;
	grp = init_grp();
	init_shell();
	set_shell((~ICANON & ~ECHO));
	stock_env(grp, env) ? shlvl(grp) : 0;
	stock_hist(grp);
	sig_handler();
	unlink(TMP_FROM);
	while (7)
		parse_cmd(0, grp);
	return (0);
}
