/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strintchr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/23 19:19:55 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/03 16:47:05 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strintchr(char *s, int tofind)
{
	size_t	i;

	i = -1;
	while (s[++i] != '\0')
		if (s[i] == tofind)
			return (i);
	return (-1);
}
