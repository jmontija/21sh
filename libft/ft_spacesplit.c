/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spacesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 22:25:15 by jmontija          #+#    #+#             */
/*   Updated: 2016/03/05 03:16:53 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_cnt_parts(const char *s)
{
	int		cnt;
	int		in_substring;

	in_substring = 0;
	cnt = 0;
	while (*s != '\0')
	{
		if (in_substring == 1 && ft_isspace(*s))
			in_substring = 0;
		if (in_substring == 0 && !ft_isspace(*s))
		{
			in_substring = 1;
			cnt++;
		}
		s++;
	}
	return (cnt);
}

static int		ft_wlen(const char *s)
{
	int		len;

	len = 0;
	while (!ft_isspace(*s) && *s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

char			**ft_spacesplit(char const *s)
{
	char	**t;
	int		nb_word;
	int		index;

	index = 0;
	nb_word = ft_cnt_parts((const char *)s);
	t = (char **)malloc(sizeof(*t) * (ft_cnt_parts((const char *)s) + 1));
	if (t == NULL)
		return (NULL);
	while (nb_word--)
	{
		while (ft_isspace(*s) && *s != '\0')
			s++;
		t[index] = ft_strsub((const char *)s, 0, ft_wlen((const char *)s));
		if (t[index] == NULL)
			return (NULL);
		s = s + ft_wlen(s);
		index++;
	}
	t[index] = NULL;
	return (t);
}
