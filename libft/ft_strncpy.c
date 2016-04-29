/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 19:32:28 by jmontija          #+#    #+#             */
/*   Updated: 2016/02/24 00:36:52 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while ((i < n) && src[i] != '\0')
	{
		dest[i] = (char)src[i];
		i++;
	}
	if (n > i)
	{
		while (n > i)
		{
			dest[i] = '\0';
			i++;
		}
	}
	return (dest);
}
