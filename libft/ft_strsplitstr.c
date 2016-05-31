/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitstr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 19:59:55 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/31 19:26:39 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int		ft_cnt_words(char *cmd, char *symbol)
{
	int symlen;
	int	cnt;
	int	i;

	i = 0;
	cnt = 0;
	symlen = ft_strlen(symbol);
	while (cmd[i] != '\0')
	{
		if ( (symlen > 1 && strncmp(cmd + i, symbol, symlen) == 0) ||
			(symlen == 1 && *symbol == cmd[i] && cmd[i + 1] != '&') )
		{
			cnt++;
			i += symlen;
		}
		else
			i++;
	}
	return (cnt + 1);
}

char		**ft_fillstr(char **t, char *cmd, char *symbol)
{
	int		i;
	int		pos;
	int		idx;
	size_t	symlen;
	int		synth = 0;

	i = 0;
	pos = 0;
	idx = 0;
	symlen = ft_strlen(symbol);
	while (cmd[i] != '\0')
	{
		if (synth == 0 && cmd[i] == '"')
			synth = 1;
		else if (synth == 1 && cmd[i] == '"')
			synth = 0;
		if (synth == 0 &&
			((symlen > 1 && strncmp(cmd + i, symbol, symlen) == 0) ||
			(symlen == 1 && *symbol == cmd[i] && cmd[i + 1] != '&')) )
		{
			t[idx] = ft_strsub(cmd, pos, i - pos);
			i += symlen;
			pos = i;
			idx++;
		}
		else
			i++;
	}
	t[idx] = ft_strsub(cmd, pos, i - pos);
	t[idx + 1] = NULL;
	return (t);
}

char			**ft_strsplitstr(char *cmd, char *symbol)
{
	char	**t;
	int		nb_word;
	int		i;

	i = -1;
	nb_word = ft_cnt_words(cmd, symbol);
	t = (char **)malloc(sizeof(char *) * nb_word + 1);
	if (t == NULL)
		return (NULL);
	t = ft_fillstr(t, cmd, symbol);
	while (t[++i])
		t[i] = ft_strtrim(t[i]);
	return (t);
}