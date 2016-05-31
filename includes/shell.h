/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmontija <jmontija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 16:50:31 by jmontija          #+#    #+#             */
/*   Updated: 2016/05/31 17:06:37 by jmontija         ###   ########.fr       */
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
# define KEY(a, b, c, d) ((d << 24) + (c << 16) + (b << 8) + a)
# define TMP_FILE		"/private/tmp/.tmp_file"
# define CAT_TMP_FILE	"cat /private/tmp/.tmp_file"
# define TMP_FROM		"/private/tmp/.tmp_from"
# define BUF_SIZE 1500
# define START_POS 7
# define ENTER 10
# define SPACE 32
# define TAB 9
# define ESC 27
# define ARROW_L 4479771
# define ARROW_U 4283163
# define ARROW_R 4414235
# define ARROW_D 4348699
# define BACKSPACE 127
# define DEL 2117294875
# define CTRL_D 4
# define CTRL_L 12
# define CTRL_R 18
# define CTRL_V 22

typedef int				t_bool;
enum {P, S, ii, v, u};
enum {e_opt, namenv, utils};

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
	char				*symbol;
	int					action;
	char				*command;
	struct s_redir		*next;
}						t_redir;

typedef struct			s_group
{
	char					**cmd;
	char					**env;
	int						*define_cmd;

	char					*order;
	char					*curr_cmd;
	char					*curr_pipe_cmd;
	int						pipe;
	int						fd_in_save;
	int						last_fd_in_save;
	int						curs_col;

	struct termios			cpy_term;
	struct s_options		*options;
	struct s_env			*first;
	struct s_env			*last;
	struct s_hist			*hist;
	struct s_hist			*curr_hist;
	struct s_redir			**redirect;
}						t_group;

t_group					*init_grp(void);
int						init_shell(void);
int						set_shell(int lflag);
int						reset_shell(void);
void					parse_cmd(int const fd, t_group *grp);
void					read_cmd(t_group *grp, int fd, char **cmd);
int						check_synth_cmd(t_group *grp);
void					create_process(t_group *grp);
int						exec_cmd(t_group *grp, char *path, char **cmd_line);
void					exec_env(t_group *grp, int show_env);
void					handling_arrow(t_group *grp, char **cmd, int key);
void					handling_backspace(t_group *grp, char **cmd);
void					remove_line(t_group *grp, char **cmd);
void					insert_hist(t_group *grp, char *name);
void					cderr_pwd(t_group *grp, char *path, struct stat s_buf);
void					ft_tputs(char *cap_code, char *what);
char					*child_process(t_group *grp, char *order);
char					*search_exec(t_group *grp, char *cmd);
char					*ft_getenv(t_group *grp, char *tofind);
int						check_parenthese(char cmd, int synth);
int 					ft_parsing(int exec, char *to_pars);
char					*ft_findocc(int idx, char *order, char *symbol);
int						main_redirection(t_group *grp, char **split_cmd, char *symbol);
int						exec_redir(int exec, t_group *grp, char *cmd);
int						manage_redirections(int exec, t_group *grp, char *pipe_cmd);
char					*get_cmd(t_group *grp, char *cmd);
int						main_pipe(t_group *grp, char **split_cmd);
void					split_exec_cmd(t_group *grp, char *cmd_to_exec, char *print);
int						list_to_tab(t_env *env, char ***env_tab);
int						exec_builtin(int exec, t_group *grp, char *order);
int						is_env(char *env);
int						insert_env(t_group *grp, char *env);
int						unset_env(t_group *grp, char *todel);
int						manage_opt(t_group *grp);
int						error_opt(char opt, char *what);
void					error_cmd(char *what, char *who);
int						error_synthax(char *error, char *file);

#endif
