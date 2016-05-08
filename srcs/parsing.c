/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 18:38:38 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/08 03:42:18 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*ft_strchrsym(char *str, char *tofind)
{
	size_t	i;
	char	*tocmp;

	i = -1;
	while (str[++i] != '\0')
	{
		tocmp = ft_charjoin("", str[i]);
		if (str[i + 1] && str[i] == str[i + 1])
		{
			tocmp = ft_charjoin(tocmp, str[i + 1]);
			i++;
		}
		if (strcmp(tocmp, tofind) == 0)
			return (tofind);
	}
	return (NULL);
}

char	*ft_findocc(char *order, char *symbol)
{
	size_t	i;
	char	**tofind;

	i = -1;
	tofind = ft_spacesplit(symbol);
	while (tofind[++i] != NULL)
		if (ft_strchrsym(order, tofind[i]) != NULL)
			return (tofind[i]);
	return (NULL);
}

// ls | CMD0 > FILE | CMD1 | CMD2 > FILE2 < OK
// ls -i DIR > FILE | CMD1 | CMD2 > FILE2 < OK
// checker si cest pas mieux de split sur les pipe first

int		ft_parsing(int exec, char *to_pars)
{
	t_group	*grp;
	int 	i;
	char	*splitw;
	char	**split_cmd;

	i = -1;
	grp = init_grp();
	if ((splitw = ft_findocc(to_pars, "> >> | < <<")) == NULL)
		return (-1);
	split_cmd = ft_strsplit(to_pars, splitw[0]);
	while (split_cmd[++i])
	{
		split_cmd[i] = ft_strtrim(split_cmd[i]);
		ft_putendl(split_cmd[i]);
	}
	exec && splitw[0] != '|' ? main_redirection(grp, split_cmd, splitw) : main_pipe(grp, split_cmd);
	return (splitw[0]);
}
