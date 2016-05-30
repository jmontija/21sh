/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/29 19:02:34 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/30 19:38:02 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


int		check_pipe(t_group *grp, char cmd, int synth)
{
	if (synth == 0 && cmd == '|')
	{
		synth = 1;
		grp->pipe += 1;
	}
	else if (synth == 1 && cmd != '|')
		synth = 0;
	else if (synth == 1 && cmd == '|')
		synth = -1;
	return (synth);
}

int		finalize_cmd(t_group *grp, int synth)
{
	int		i;
	char	*order;

	while (synth)
	{
		order = SDUP("");
		ft_putstr_fd("\033[1;34m", 2);
		ft_putstr_fd("pipe> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
		read_cmd(grp, 0, &order);
		grp->order = ft_charjoin(grp->order, '\n');
		grp->order = JOIN(grp->order, order);
		i = -1;
		while (order[++i])
		{
			synth = check_pipe(grp, order[i], synth);
			if (synth < 0)
				return (error_synthax("error parsing near", "|"));
		}
		REMOVE(&order);
	}
	return (0);
	//printf("ORDER = %s\n", grp->order);
}

int		check_synth_pipe(t_group *grp, char **split_cmd)
{
	int			i;
	int			j;
	char		*path;
	char		**pipe_cmd;
	struct stat	s_buf;

	i = -1;
	while (split_cmd[++i])
	{
		if (split_cmd[i][0] == '>' || split_cmd[i][0] == '<')
			return (error_synthax("error parsing near", "|"));
		pipe_cmd = ft_spacesplit(get_cmd(grp, split_cmd[i]));
		j = -1;
		while (pipe_cmd[++j])
			pipe_cmd[j] = ft_strtrim(pipe_cmd[j]);
		grp->cmd = pipe_cmd;
		path = child_process(grp, NULL);
		if (path && lstat(path, &s_buf) < 0)
			return (error_synthax("unknown command", pipe_cmd[0]));
		if (i == grp->pipe)
			return (0);
	}
	if (i < grp->pipe)
		return (error_synthax("error parsing near", "|"));
	return (finalize_cmd(grp, 1));
}

void	is_parenthese_closed(t_group *grp, int synth)
{
	char	*order;
	int		i;

	while (synth)
	{
		order = SDUP("");
		ft_putstr_fd("\033[1;34m", 2);
		ft_putstr_fd("quote> ", 2);
		ft_putstr_fd("\033[1;37m", 2);
		read_cmd(grp, 0, &order);
		grp->order = ft_charjoin(grp->order, '\n');
		grp->order = JOIN(grp->order, order);
		i = -1;
		while (order[++i])
			synth = check_parenthese(order[i], synth);
		REMOVE(&order);
	}
	printf("ORDER RECOMPOSED = %s\n", grp->order);
}

int		check_synth_cmd(t_group *grp)
{
	int	i;
	int synth;

	i = -1;
	synth = false;
	while (grp->order[++i] != '\0')
	{
		synth = check_parenthese(grp->order[i], synth);
		if (synth == false && grp->order[i] == '|')
			grp->pipe += 1;
	}
	is_parenthese_closed(grp, synth); // <- attention verifier sil ne rajoute pas de pipe
	if (grp->pipe > 0)
		return (check_synth_pipe(grp, ft_strsplitstr(grp->order, "|"))); // <- attention verifier sil ne rajoute pas de "guillemets"
	return (0);
}