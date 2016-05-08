/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_charjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 18:13:26 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/08 03:41:52 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_charjoin(char *s1, char c)
{
	char	*new;
	size_t	size;

	size = ft_strlen(s1) + 1;
	new = ft_strnew(size);
	new = ft_strcpy(new, s1);
	new[size - 1] = c;
	new[size] = '\0';
	return (new);
}
