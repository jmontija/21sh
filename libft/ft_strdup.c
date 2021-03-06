/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 14:19:53 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/22 01:49:26 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*dup;

	i = 0;
	if (s1)
	{
		dup = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
		while (s1[i] != '\0')
		{
			dup[i] = s1[i];
			i++;
		}
		dup[i] = '\0';
		return (dup);
	}
	return (NULL);
}
