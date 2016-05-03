/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lib.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:06:08 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/03 16:25:30 by jmontija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		ft_getchar(int c)
{
	return (write(2, &c, 1));
}

void	ft_tputs(char *cap_code, char *cap_code_bis)
{
	t_group	*grp;

	grp = init_grp();
	if (cap_code_bis == NULL)
		tputs(tgetstr(cap_code, NULL), 1, ft_getchar);
	else
		tputs(tgoto(tgetstr(cap_code_bis, NULL), 0, grp->curs_col), 1, ft_getchar);
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
