/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:34:10 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/20 09:27:52 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include "../includes/minishell.h"
typedef struct s_env		t_env;
typedef struct s_cmd		t_cmd;
typedef struct s_malloc		t_malloc;

# define PWD_ERROR "pwd: error retrieving current directory:\
getcwd: cannot access parent directories: No such file or directory"

typedef struct s_tools
{
	t_cmd		*cmd;
	t_malloc	**aloc;
	t_env		**env;
	int			*r_stat;
	char		**envp;
}	t_tools;

t_env	*get_bef_node(t_env *lst, t_env *node);
void	cd_error(char *path);
void	cmd_file_error(char *path, char *msg);
void	execve_error(char *cmd);
void	export_display(t_env **env, t_malloc **a);
int		ft_echo(t_cmd *cmd);
int		ft_pwd(t_env **env, t_malloc **a, char *to_path, t_tools *tools);
int		ft_export(t_tools *tools);
int		ft_cd(t_tools *tools);
int		ft_env(t_malloc **aloc, t_env **env, t_cmd *cmd);
void	ft_exit(t_malloc **aloc, t_cmd *cmd, int *status);
int		ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env);
int		var_action(char *key, char *value, t_env *env);
int		is_var_exist(char *key, t_env *vars);
void	update_var(t_tools *tools, char *new_value, char *key);
int		is_key_valid(char *key);
void	remove_variable(char *key, t_env **vars, t_malloc **alloc);
void	append_value(t_tools *tools, char *key, char *value);
char	**keys_to_2darray(t_env *env, t_malloc **alloc);
char	**sort_2d_array(char **arr);
void	execute_builtin(int i, t_tools *tools);
int		is_builtins(char *str);
char	*get_executable_path(char *str, t_malloc **alloc, t_env *env);
void	ft_execute_simple_cmd(t_tools *tools);
void	execute_pipeline(t_tools *tools);
void	execute_piped_cmd(t_tools *tools);
void	redirect_command(t_cmd *cmd);
void	ft_execute(t_tools *tools);
char	**vars_to_envp(t_tools *tools);

#endif
