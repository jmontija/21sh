/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitquote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 19:59:55 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/19 04:12:57 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int		ft_cnt_parts(const char *s, char c)
{
	int		cnt;
	int		synth;
	int		in_part;

	in_part = 0;
	synth = 0;
	cnt = 0;
	while (*s != '\0')
	{
		synth = check_parentheses(*s);
		if (synth == 0 && in_part == 1 && *s == c)
			in_part = 0;
		if (in_part == 0 && *s != c)
		{
			in_part = 1;
			cnt++;
		}
		s++;
	}
	return (cnt);
}

static int		ft_wlen(const char *s, char c)
{
	int		len;
	int		synth;

	len = 0;
	synth = 0;
	while (*s != '\0')
	{
		synth = check_parentheses(*s);
		if (synth == 0 && *s == c)
			break ;
		len++;
		s++;
	}
	return (len);
}

char			**ft_strsplitquote(char *s, char c)
{
	char	**t;
	int		nb_word;
	int		index;

	index = 0;
	if (s == NULL)
		return (NULL);
	nb_word = ft_cnt_parts((const char *)s, c);
	t = (char **)malloc(sizeof(*t) * nb_word + 1);
	if (t == NULL)
		return (NULL);
	while (nb_word--)
	{
		while (*s == c && *s != '\0')
			s++;
		t[index] = ft_strsub((const char *)s, 0, ft_wlen((const char *)s, c));
		if (t[index] == NULL)
			return (NULL);
		s = s + ft_wlen(s, c);
		index++;
	}
	t[index] = NULL;
	return (t);
}
