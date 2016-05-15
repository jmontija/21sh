/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 18:38:38 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/15 20:38:18 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*ft_strchrsym(char *to_pars, char *tofind)
{
	size_t	i;
	char	*tocmp;
	int synth = false;

	i = -1;
	while (to_pars[++i] != '\0')
	{
		synth = check_parenthese(to_pars[i], synth);
		tocmp = ft_charjoin("", to_pars[i]);
		if (to_pars[i + 1] && to_pars[i] == to_pars[i + 1])
		{
			tocmp = ft_charjoin(tocmp, to_pars[i + 1]);
			i++;
		}
		if (synth == 0 && strcmp(tocmp, tofind) == 0)
			return (tofind);
	}
	return (NULL);
}

char	*ft_findocc(int idx, char *to_pars, char *symbol)
{
	size_t	i;
	char	**tofind;

	i = -1;
	tofind = ft_spacesplit(symbol);
	while (tofind[++i] != NULL)
		if (ft_strchrsym(to_pars, tofind[i]) != NULL)
			return (idx ? ft_itoa(i) : tofind[i]);
	return (NULL);
}

int		ft_parsing(int exec, char *to_pars)
{
	t_group	*grp;
	int 	i;
	char	*splitw;
	char	**split_cmd;

	i = -1;
	grp = init_grp();
	if ((splitw = ft_findocc(false, to_pars, "| >> > < <<")) == NULL)
		return (-1);
	printf("PARSER -> '%s'\n", splitw);
	split_cmd = ft_strsplitstr(to_pars, splitw);
	if (exec)
		splitw[0] != '|' ? main_redirection(grp, split_cmd, splitw) : main_pipe(grp, split_cmd);
	return (splitw[0] == '|' ? 0 : -1);
}
