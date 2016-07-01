/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/14 15:57:08 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/22 22:46:56 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	manage_env(t_group *grp)
{
	int		i;
	int		j;
	int		pos;

	i = -1;
	j = -1;
	pos = list_to_tab(0, grp->first, NULL);
	while (++i < grp->define_cmd[namenv] &&
		CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1])
		pos += 1;
	grp->env = (char **)malloc(sizeof(char *) * (pos + 1));
	grp->env_save = (char **)malloc(sizeof(char *) * (pos + 1));
	while (++j < pos + 1)
	{
		grp->env[j] = NULL;
		grp->env_save[j] = NULL;
	}
	i = -1;
	pos = list_to_tab(1, grp->first, &(grp->env));
	while (++i < grp->define_cmd[namenv] &&
		CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1])
	{
		grp->env[pos + i] =
		SDUP(CMD(cmd_split)[i + grp->define_cmd[e_opt] + 1]);
	}
}

void	define_arg_cmd(t_group *grp)
{
	int	i;

	i = 0;
	while (CMD(cmd_split) && CMD(cmd_split)[++i])
	{
		if (CMD(cmd_split)[i][0] == '-' &&
			grp->define_cmd[namenv] == false &&
			grp->define_cmd[utils] == false)
			grp->define_cmd[e_opt] = i;
		else if (is_env(CMD(cmd_split)[i]) > 0 &&
			grp->define_cmd[utils] == false)
			grp->define_cmd[namenv] = i;
		else
		{
			grp->define_cmd[utils] = i;
			break ;
		}
	}
}

void	get_save_cmd(int idx_cmd, t_group *grp)
{
	t_redir *curr;

	curr = grp->sh_cmd[idx_cmd];
	while (curr != NULL)
	{
		if (curr->symbol == NULL)
		{
			grp->cmd_save = curr;
			return ;
		}
		curr = curr->next;
	}
	ft_putendl_fd("error CMD", 2);
}

int		manage_pre_exec(int idx_cmd, t_group *grp, char *pipe_cmd)
{
	int		ret;

	ret = 0;
	pipe_cmd = NULL;
	get_save_cmd(idx_cmd, grp);
	define_arg_cmd(grp);
	manage_env(grp);
	redir_from(idx_cmd, grp);
	if (grp->exit[1] == false && grp->sh_cmd[idx_cmd + 1] == NULL)
		ret = redir_to(idx_cmd, grp);
	return (ret);
}
