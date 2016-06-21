/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/19 03:11:24 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/19 04:29:10 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int		ret_clean_quotes(int free, int *quotes)
{
	int	i;

	i = -1;
	while (quotes && ++i < 6)
	{
		if (free == 0 && quotes[i] > 0)
			return (1);
		else if (free == 0 && quotes[i] < 0)
			return (-1);
		else if (free == 1)
			quotes[i] = 0;
	}
	return (0);
}

void	check_quotes(int *quotes, char c, char type, int idx)
{
	if (c != type)
		return ;
	else if ((type == '{' || type == '[' || type == '(') ||
		((type == '\'' || type == '"') && quotes[idx] == 0))
		quotes[idx] += 1;
	else
		quotes[idx] -= 1;
}

int		malloc_free(int **quotes, char c)
{
	int	i;

	i = -1;
	if (c == 0)
		return (ret_clean_quotes(1, *quotes));
	if (*quotes == NULL)
	{
		*quotes = (int *)malloc(sizeof(int) * 6);
		while (++i < 6)
			(*quotes)[i] = 0;
	}
	return (0);
}

int		check_parentheses(char c)
{
	int			ret;
	static int	*quotes = NULL;

	ret = 0;
	if (malloc_free(&quotes, c) < 0)
		return (ret);
	quotes[dquote] == 0 ? check_quotes(quotes, c, '\'', squote) : 0;
	quotes[squote] == 0 ? check_quotes(quotes, c, '"', dquote) : 0;
	check_quotes(quotes, c, '`', bquote);
	if (quotes[squote] == 0 && quotes[dquote] == 0)
	{
		check_quotes(quotes, c, '{', acc);
		check_quotes(quotes, c, '}', acc);
		check_quotes(quotes, c, '[', cro);
		check_quotes(quotes, c, ']', cro);
		check_quotes(quotes, c, '(', par);
		check_quotes(quotes, c, ')', par);
	}
	ret = ret_clean_quotes(0, quotes);
	return (ret);
}
