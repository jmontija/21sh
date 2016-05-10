/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitstr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 19:59:55 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/10 22:03:50 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int		ft_cnt_parts(const char *s, char *c)
{
	int		cnt;
	int		in_substring;
	char	*tocmp;

	in_substring = 0;
	cnt = 0;
	while (*s != '\0')
	{
		tocmp = ft_charjoin("", *s);
		if (*(s + 1) && *s == *(s + 1))
		{
			tocmp = ft_charjoin(tocmp, *(s + 1));
			s++;
		}
		if (in_substring == 1 && ft_strcmp(tocmp, c) == 0)
			in_substring = 0;
		if (in_substring == 0 && ft_strcmp(tocmp, c) != 0)
		{
			in_substring = 1;
			cnt++;
		}
		s++;
	}
	return (cnt);
}

/*
static int		ft_wlen(const char *s, char *c)
{
	int		len;
	char	*tocmp;

	len = 0;
	while (*s != '\0')
	{
		tocmp = ft_charjoin("", *s);
		if (*(s + 1) && *s == *(s + 1))
		{
			tocmp = ft_charjoin(tocmp, *(s + 1));
			s++;
		}

		if (ft_strcmp(tocmp, c) == 0)
			break ;
		s++;
		len++;
	}
	//printf("LEN %d WORD %s\n", len, s);
	return (len);
}
*/

char			**ft_strsplitstr(char const *s, char *c)
{
	char	**t;
	int		nb_word;
	int		index;
	char	*tocmp;

	index = 0;
	if (s == NULL)
		return (NULL);
	nb_word = ft_cnt_parts((const char *)s, c);
	printf("WORD = %d\n", nb_word);
	t = (char **)malloc(sizeof(*t) * nb_word + 1);
	if (t == NULL)
		return (NULL);
	while (nb_word--)
	{
		while (*s != '\0')
		{
			tocmp = ft_charjoin("", *s);
			if (*(s + 1) && *s == *(s + 1))
			{
				tocmp = ft_charjoin(tocmp, *(s + 1));
				s++;
			}
			if (ft_strcmp(tocmp, c) != 0)
				break ;
			s++;
		}
		t[index] = ft_strsub((const char *)s, 0, 5);
		printf("WORD = %s\n", t[index]);
		if (t[index] == NULL)
			return (NULL);
		s = s + 5;
		index++;
	}
	t[index] = NULL;
	return (t);
}