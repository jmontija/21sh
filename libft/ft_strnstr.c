/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 20:20:45 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/12 00:46:24 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t n)
{
	size_t	litlen;

	if (*little == '\0')
		return ((char *)big);
	litlen = ft_strlen(little);
	while (*big != '\0' && n-- >= litlen)
	{
		if (*big == *little && ft_strncmp(big, little, litlen) == 0)
			return ((char *)big);
		big++;
	}
	return (NULL);
}
