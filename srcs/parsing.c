/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 18:38:38 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/13 22:22:16 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*get_cmd(t_group *grp, char *cmd)
{
	char	**symbol;
	char	*shell_cmd;
	char	*tofind;
	size_t 	symlen;
	size_t		i;
	int			j;

	i = -1;
	tofind = SDUP("> >> < <<");
	symbol = ft_spacesplit(tofind);
	shell_cmd = SDUP(cmd);
	printf("GET COMMAND\n");
	while (cmd[++i] != '\0')
	{
		j = -1;
		while (symbol[++j] != NULL)
		{
			symlen = ft_strlen(symbol[j]);
			if ( (symlen > 1 && strncmp(cmd + i, symbol[j], symlen) == 0) ||
				(symlen == 1 && *symbol[j] == cmd[i] && cmd[i - 1] && cmd[i + 1] &&
					cmd[i + 1] != *symbol[j] && cmd[i - 1] != *symbol[j]) )
			{
				shell_cmd = SDUP(ft_strsplitstr(cmd, symbol[j])[0]);
				return (shell_cmd);
			}
		}
	}
	return (shell_cmd);
}

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
