/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio <julio@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 16:50:31 by jmontija          #+#    #+#             */
/*   Updated: 2016/06/17 02:40:53 by julio            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include "libft.h"
# include "get_next_line.h"
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <termios.h>
# include <term.h>
# include <signal.h>
# include <curses.h>
# include <sys/ioctl.h>

# define NEW(x)			(ft_strnew(x))
# define LEN(x)			(ft_strlen(x))
# define SDUP(x)		(ft_strdup(x))
# define SUB(a, b, c)	(ft_strsub(a, b, c))
# define JOIN(a, b)		(ft_strjoin(a, b))
# define STRCHR(a, b)	(ft_strchr(a, b))
# define REMOVE(x)		(ft_strdel(x))
# define CMD(x)			((grp->cmd_save)->x)
# define TERM(x)		((grp->term)->x)
# define KEY(a, b, c, d) ((d << 24) + (c << 16) + (b << 8) + a)
//# define TMP_FROM		"/private/tmp/.tmp_from"
# define FD_DIR			"/dev/fd/0"
# define FD_0_DIR		"/dev/pts/1"
# define TMP_FROM		"/tmp/.tmp_from"
# define BUF_SIZE 4096
# define START_POS 6
# define ENTER 10
# define SPACE 32
# define TAB 9
# define ESC 27
# define ARROW_L 4479771
# define ARROW_U 4283163
# define ARROW_R 4414235
# define ARROW_D 4348699
# define ALT_L 1146821403
# define ALT_R 1130044187
# define BACKSPACE 127
# define DEL 2117294875
# define CTRL_D 4
# define CTRL_L 12
# define CTRL_R 18
# define CTRL_V 22
# define PAGE_UP 2117425947
# define PAGE_DOWN 2117491483
# define HOME 4741915
# define END 4610843

typedef int				t_bool;
enum {P, S, ii, v, u};
enum {e_opt, namenv, utils};
enum {squote, dquote, bquote, par, cro, acc};

typedef struct			s_env
{
	char				*name;
	char				*val;
	t_bool				exist;
	struct s_env		*next;
}						t_env;

typedef struct			s_options
{
	t_bool				*on;
	char				**params;
}						t_options;

typedef struct			s_hist
{
	char				*name;
	struct s_hist		*prev;
	struct s_hist		*next;
}						t_hist;

typedef struct			s_redir
{
	char				*name;
	char				**cmd_split;
	char				*symbol;
	int					fd;
	struct s_redir		*next;
}						t_redir;

typedef	struct			s_window
{
	int					width;
	int					heigth;
}						t_window;

typedef struct			s_term
{
	char				*cmd_line;
	int					curs_pos;
	int					cmd_size;
	int					other_read;
	struct s_window		*window;
}						t_term;

typedef struct			s_group
{
	char					**cmd;
	char					**env;
	char					**env_save;
	int						*define_cmd;

	char					*order;
	char					*curr_cmd;
	int						pipe;
	int						fd_in_save;
	int						fd_out_save;
	int						fd_tmp;
	int						*quotes;
	int						exit[2];
	struct s_redir			**sh_cmd;
	struct s_redir			*cmd_save;
	struct s_term			*term;

	struct termios			cpy_term;
	struct s_options		*options;
	struct s_env			*first;
	struct s_env			*last;
	struct s_hist			*hist;
	struct s_hist			*curr_hist;
}						t_group;

t_group					*init_grp(void);
int						init_shell(void);
int						set_shell(int lflag);
int						reset_shell(void);
void					parse_cmd(int const fd, t_group *grp);
void					read_cmd(t_group *grp, int fd);
int						check_synth_cmd(t_group *grp);
int						check_file(t_group *grp, char *name, int rights);
void					exec_env(t_group *grp, int show_env);
int						exec_command(int fd, t_group *grp, int all_fd);
void					handling_arrow(t_group *grp, int key);
void					handling_backspace(t_group *grp);
void					handling_clear_screen(t_group *grp);
void					handling_ctrl_d(t_group *grp);
void					remove_line(t_group *grp);
void					insert_hist(t_group *grp, char *name);
void					cderr_pwd(t_group *grp, char *path, struct stat s_buf);
char					*child_process(t_group *grp, char *order);
char					*search_exec(t_group *grp, char *cmd);
char					*ft_getenv(t_group *grp, char *tofind);
int						check_parentheses(t_group *grp, char cmd);
int						manage_pre_exec(int idx_cmd, t_group *grp, char *pipe_cmd);
int						main_pipe(t_group *grp, char **split_cmd);
int						list_to_tab(int stock, t_env *env, char ***env_tab);
int						exec_builtin(int exec, t_group *grp, char *order);
int						is_env(char *env);
int						insert_env(t_group *grp, char *env);
int						unset_env(t_group *grp, char *todel);
int						manage_opt(t_group *grp);
int						error_opt(char opt, char *what);
void					error_cmd(char *what, char *who);
int						error_synthax(char *error, char *file);
int						insert_fd(int idx_cmd, t_group *grp, char *file, char *symbol);
int						insert_cmd(int idx_cmd, t_group *grp);
int						ft_getchar(int c);
void					print_cmd(t_group *grp, char *order);
void					ft_go_home(t_group *grp);
void					ft_go_end(t_group *grp);
void					ft_go_up(t_group *grp);
void					ft_go_down(t_group *grp);
void					ft_left_arrow(t_group *grp);
void					ft_right_arrow(t_group *grp);
void					ft_prev_word(t_group *grp);
void					ft_next_word(t_group *grp);
void					redir_from(int idx_cmd, t_group *grp);
int						redir_to(int idx_cmd, t_group *grp);
#endif
