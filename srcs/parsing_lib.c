/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 18:43:19 by julio             #+#    #+#             */
/*   Updated: 2016/06/08 17:33:36 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		ret_clean_quotes(int free, t_group *grp)
{
	int	i;

	i = -1;
	while (++i < 6)
	{
		if (free == false && grp->quotes[i] > 0)
			return (1);
		else if (free == false && grp->quotes[i] < 0)
			return (-1);
		else if (free == true)
			grp->quotes[i] = 0;
	}
	/*if (free == true)
		ft_putendl("grp->quotes REINITIALIZED");*/
	return (0);
}

void	check_quotes(t_group *grp, char c, char type, int idx)
{
	if (c != type)
		return ;
	else if ((type == '{' || type == '[' || type == '(') || ((type == '\'' || type == '"') && grp->quotes[idx] == 0))
		grp->quotes[idx] += 1;
	else
		grp->quotes[idx] -= 1;
}

int		check_parentheses(t_group *grp, char c)
{
	int		i;
	int		ret;

	i = -1;
	ret = 0;
	if (c == 0)
		return (ret_clean_quotes(1, grp));
	grp->quotes[dquote] == 0 ? check_quotes(grp, c, '\'', squote) : 0;
	grp->quotes[squote] == 0 ? check_quotes(grp, c, '"', dquote) : 0;
	check_quotes(grp, c, '`', bquote);
	if (grp->quotes[squote] == 0 && grp->quotes[dquote] == 0)
	{
		check_quotes(grp, c, '{', acc);
		check_quotes(grp, c, '}', acc);
		check_quotes(grp, c, '[', cro);
		check_quotes(grp, c, ']', cro);
		check_quotes(grp, c, '(', par);
		check_quotes(grp, c, ')', par);
	}
	ret = ret_clean_quotes(0, grp);
	return (ret);
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