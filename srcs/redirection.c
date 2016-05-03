/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 02:03:59 by julio             #+#    #+#             */
/*   Updated: 2016/05/03 02:38:56 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	parsing(char **cmd, int idx, int occ)
{
	 /* cut in the middle
	 */
	last_params = SUB();
	next_params = SUB();

}

char 	**redirections(char **cmd_line)
{
	int	fd;
	int	occ;
	int i;

	i = -1;
	occ = -1;
	while (cmd_line[++i])
	{
		if ((occ = ft_strintchr(cmd_line[i], '>')) >= 0) /* modifier fonction */
		{	
			cmd_line = parsing(cmd_line, i, occ);
			fd = open(cmd_line[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			dup2(fd ,1);
			break ;
		}
	}
	close(fd);
	return (cmd_line);
}