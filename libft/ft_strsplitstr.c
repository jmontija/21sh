/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitstr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 19:59:55 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/12 00:40:52 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int		ft_cnt_words(char *cmd, char *symbol)
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
			(symlen == 1 && *symbol == cmd[i] && cmd[i - 1] && cmd[i + 1] &&
				cmd[i + 1] != *symbol && cmd[i - 1] != *symbol) )
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
	
	i = 0;
	pos = 0;
	idx = 0;
	symlen = ft_strlen(symbol);
	while (cmd[i] != '\0')
	{
		if ( (symlen > 1 && strncmp(cmd + i, symbol, symlen) == 0) ||
			(symlen == 1 && *symbol == cmd[i] && cmd[i - 1] && cmd[i + 1] &&
				cmd[i + 1] != *symbol && cmd[i - 1] != *symbol) )
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
	printf("WORD = %d\n", nb_word);
	t = ft_fillstr(t, cmd, symbol);
	while (t[++i])
		t[i] = ft_strtrim(t[i]);
	return (t);
}