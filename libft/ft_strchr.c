/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 18:27:04 by jmontija          #+#    #+#             */
/*   Updated: 2016/02/06 11:04:35 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	len;
	char	*str;
	char	occ;

	i = 0;
	len = ft_strlen(s);
	str = (char *)s;
	occ = (char)c;
	while (i <= len)
	{
		if (str[i] == occ)
			return (str + i);
		i++;
	}
	return (NULL);
}
