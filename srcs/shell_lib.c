/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lib.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:06:08 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/20 23:38:06 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	show_prompt(t_group *grp, char *prompt, int size, char *color)
{
	ft_putstr_fd(color, 2);
	ft_putstr_fd(prompt, 2);
	ft_putstr_fd("\033[1;37m", 2);
	grp->prompt_size = size;
}

int		ft_getchar(int c)
{
	return (write(2, &c, 1));
}

int		reset_shell(void)
{
	t_group	*grp;

	grp = init_grp();
	if (tcsetattr(0, 0, &(grp->cpy_term)) == -1)
		return (-1);
	return (0);
}

int		set_shell(int lflag)
{
	struct termios	term;
	t_group			*grp;

	grp = init_grp();
	if (tcgetattr(0, &term) == -1)
		return (-1);
	grp->cpy_term = term;
	term.c_lflag = term.c_lflag & lflag;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term))
		return (-1);
	return (0);
}

int		init_shell(void)
{
	char	*name;

	if ((name = getenv("TERM")) == NULL ||
		(ft_strcmp(name, "xterm-256color") != 0))
		name = "xterm-256color";
	if (tgetent(NULL, name) == ERR)
		return (-1);
	return (0);
}
