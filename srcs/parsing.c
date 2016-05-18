/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 18:38:38 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/18 23:27:33 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*ft_strchrsym(char *to_pars, char *tofind)
{
	size_t	i;
	char	*tocmp;
	int 	synth;
	int		symlen;

	i = -1;
	synth = false;
	symlen = ft_strlen(tofind);
	while (to_pars[++i] != '\0')
	{
		synth = check_parenthese(to_pars[i], synth);
		if (synth == 0 &&
			((symlen > 1 && ft_strncmp(to_pars + i, tofind, symlen) == 0) ||
			(symlen == 1 && tofind[0] == to_pars[i] && to_pars[i + 1] != '&'/*&& to_pars[i - 1] && to_pars[i + 1] &&
			to_pars[i + 1] != *tofind && to_pars[i - 1] != *tofind*/)) )
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

char	*check_close_fd(char *to_pars, char **splitw, char **split_cmd)
{
	int	i;

	while (LEN(*splitw) > 2)
	{
		printf("NEED TO CLOSE OR REDIRECT FD %s\n", *splitw);
		ft_putendl(to_pars);
		split_cmd = ft_strsplitstr(to_pars, *splitw);
		REMOVE(&to_pars);
		to_pars = SDUP("");
		i = -1;
		while (split_cmd[++i])
		{
			to_pars = JOIN(to_pars, split_cmd[i]);
			to_pars = ft_charjoin(to_pars, ' ');
		}
		ft_putendl(to_pars);
		if ((*splitw = ft_findocc(false, to_pars, "| >> > << < 1>&2 2>&1 2>&- 1>&- >&-")) == NULL)
			return (NULL);
		printf("PARSER -> '%s'\n", *splitw);
	}
	return (to_pars);
}

int		ft_parsing(int exec, char *to_pars)
{
	t_group	*grp;
	int 	i;
	char	*splitw;
	char	**split_cmd;

	grp = init_grp();
	if ((splitw = ft_findocc(false, to_pars, "| >> > << < 1>&2 2>&1 2>&- 1>&- >&-")) == NULL)
		return (-1);
	printf("PARSER -> '%s'\n", splitw);
	to_pars = check_close_fd(to_pars, &splitw, split_cmd);
	if (to_pars == NULL)
		return (-1);
	split_cmd = ft_strsplitstr(to_pars, splitw);
	if (exec)
		splitw[0] != '|' ? main_redirection(grp, split_cmd, splitw) : main_pipe(grp, split_cmd);
	return (splitw[0] == '|' ? 0 : -1);
}
