/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/23 00:02:40 by julio             #+#    #+#             */
/*   Updated: 2016/06/15 03:41:22 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		params_opt(t_group *grp, int opt)
{
	int		bin;
	char	*tmp_p;

	bin = grp->define_cmd[utils];
	if (grp->cmd[bin] == NULL || bin == false)
		return (-1);
	if (opt == P)
	{
		if (grp->cmd[bin + 1] == NULL)
			return (-1);
		tmp_p = JOIN(grp->cmd[bin], "/");
	}
	if (opt == P)
		grp->options->params[opt] = JOIN(tmp_p, grp->cmd[bin + 1]);
	else
		grp->options->params[opt] = SDUP(grp->cmd[bin]);
	grp->options->on[opt] = true;
	return (1);
}

int		adjust_opt(t_group *grp, char opt)
{
	opt == 'P' ? (grp->options->on[P] = true) : 0;
	opt == 'S' ? (grp->options->on[S] = true) : 0;
	opt == '-' ? (grp->options->on[ii] = true) : 0;
	opt == 'i' ? (grp->options->on[ii] = true) : 0;
	if (opt == 'u')
		return (params_opt(grp, u));
	else if (opt == 'v')
		return (params_opt(grp, v));
	else if (opt == 'P')
		return (params_opt(grp, P));
	return (1);
}

int		isvalid_opt(t_group *grp, char opt)
{
	size_t	i;
	char	*text;
	char	*env_options;

	i = -1;
	env_options = SDUP("-PSiuv");
	text = NEW(2);
	text[0] = '-';
	text[1] = opt;
	text[2] = '\0';
	while (++i < LEN(env_options))
	{
		if (opt == env_options[i])
		{
			if (adjust_opt(grp, opt) < 0)
			{
				return (error_opt(opt, " not enough arguments"));
			}
			return (1);
		}
	}
	return (error_opt(opt, " illegal env option or not Asked"));
}

int		analyse_opt(t_group *grp, int j)
{
	size_t	i;

	i = 0;
	if (grp->cmd[j][1] == '\0')
		isvalid_opt(grp, '-');
	while (++i < LEN(grp->cmd[j]))
	{
		if (isvalid_opt(grp, grp->cmd[j][i]) < 0)
			return (-1);
	}
	return (1);
}

int		manage_opt(t_group *grp)
{
	int	i;

	i = -1;
	while (++i < grp->define_cmd[e_opt])
		if (analyse_opt(grp, i + 1) < 0)
			return (-1);
	return (1);
}
