/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/13 16:17:39 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/02 13:42:51 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strtrim(char const *s)
{
	char			*result;
	unsigned int	i;
	unsigned int	len;

	if (s)
	{
		len = ft_len_nospace(s);
		if (len > 0)
		{
			i = 0;
			result = ft_strnew(len);
			if (result)
			{
				while (s[i] && ft_isspace(s[i]))
					i++;
				if (!s[i])
					return (ft_strnew(0));
				ft_strncpy(result, &s[i], len);
				result[len] = '\0';
				return (result);
			}
		}
		else
			return (ft_strnew(0));
	}
	return (NULL);
}
