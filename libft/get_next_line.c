/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/21 18:39:18 by jmontija          #+#    #+#             */
/*   Updated: 2016/03/29 18:02:04 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	manage_rest(char **rest, size_t i)
{
	char *old_rest;

	old_rest = *rest;
	if ((*rest)[i + 1])
	{
		*rest = SUB(*rest, i + 1, LEN(*rest));
		REMOVE(&old_rest);
	}
	else
		REMOVE(rest);
}

void	manage_line(int is_join, char **line, char **rest, size_t i)
{
	char *old_line;
	char *eol;

	if (is_join)
	{
		old_line = *line;
		eol = SUB(*rest, 0, i);
		*line = JOIN(*line, eol);
		REMOVE(&old_line);
		REMOVE(&eol);
	}
	else
		*line = SDUP("");
	manage_rest(rest, i);
}

int		check_rest(t_get *get, char **rest, char **line)
{
	size_t	i;
	char	*tmp;

	i = -1;
	get->ret = 1;
	while (*rest && ++i < LEN(*rest))
	{
		if ((*rest)[i] == '\n')
		{
			if (i == 0 && *line && LEN(*line) == 0)
				manage_line(0, line, rest, i);
			else
				manage_line(1, line, rest, i);
			return (1);
		}
	}
	tmp = *line;
	*line = JOIN(*line, *rest);
	if (*line == NULL)
		*line = SDUP(*rest);
	REMOVE(&tmp);
	REMOVE(rest);
	return (0);
}

void	ft_read(t_get *get, char **rest, char **line)
{
	char *buf;

	buf = NEW(BUFF_SIZE);
	if (buf == NULL)
	{
		get->ret = -1;
		return ;
	}
	while ((get->ret = read(get->fd, buf, BUFF_SIZE)))
	{
		buf[get->ret] = '\0';
		*rest = JOIN(*rest, buf);
		if (*rest == NULL)
			*rest = SDUP(buf);
		if ((get->ret < 0) || check_rest(get, rest, line))
			break ;
	}
	REMOVE(&buf);
}

int		get_next_line(int const fd, char **line)
{
	t_get		*get;
	static char	**rest = NULL;

	if (fd < 0 || fd >= FD_MAX || BUFF_SIZE <= 0 || !line)
		return (-1);
	if (!(get = (t_get *)malloc(sizeof(t_get))))
		return (-1);
	get->fd = fd;
	if (rest == NULL)
		if (!(rest = (char **)malloc(sizeof(char *) * FD_MAX)))
			return (-1);
	*line = NEW(0);
	if (rest[fd] == NULL || !check_rest(get, &rest[fd], line))
		ft_read(get, &rest[fd], line);
	if (get->ret == -1)
		return (-1);
	if (get->ret == 0 && rest[fd] == NULL && *line && LEN(*line) == 0)
	{
		ft_memdel((void *)&get);
		return (0);
	}
	ft_memdel((void *)&get);
	return (1);
}
