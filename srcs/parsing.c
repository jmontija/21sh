/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 18:38:38 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/12 02:20:33 by julio            ###   ########.fr       */
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

char	*ft_findocc(char *to_pars, char *symbol)
{
	size_t	i;
	char	**tofind;

	i = -1;
	tofind = ft_spacesplit(symbol);
	while (tofind[++i] != NULL)
		if (ft_strchrsym(to_pars, tofind[i]) != NULL)
			return (tofind[i]);
	return (NULL);
}

// ls | CMD0 > FILE | CMD1 | CMD2 > FILE2 < OK
// ls -i DIR > FILE | CMD1 | CMD2 > FILE2 < OK
// ls < TETS | CMD0 > FILE | CMD1 | CMD2 > FILE2 < OK



int		ft_parsing(int exec, char *to_pars)
{
	t_group	*grp;
	int 	i;
	char	*splitw;
	char	**split_cmd;

	i = -1;
	grp = init_grp();
	if ((splitw = ft_findocc(to_pars, "| > >> < <<")) == NULL)
		return (-1);
	printf("PARSER -> '%s'\n", splitw);
	split_cmd = ft_strsplitstr(to_pars, splitw);	
	if (exec)
		splitw[0] != '|' ? main_redirection(grp, split_cmd, splitw) : main_pipe(grp, split_cmd);
	return (splitw[0] != '|' ? -1 : 0);
}
