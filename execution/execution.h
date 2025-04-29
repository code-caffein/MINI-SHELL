#ifndef EXECUTION_H
#define EXECUTION_H


#include<stdio.h> 
#include<unistd.h> 
#include<string.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


#include "../includes/minishell.h"

char	*get_executable_path(char *str);
int		ft_export(t_cmd *cmd, t_malloc **aloc, t_env **env);
int		ft_cd(t_cmd *cmd, t_malloc **aloc);
void	ft_env(t_malloc **aloc, t_env **env);
void	ft_exit(t_malloc **aloc, int status);
int		ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env);
int		var_action(char *key ,char *value, t_env *env);
t_env	*get_bef_node(t_env *lst, t_env *node);
int		is_var_exist(char *key, t_env *vars);
void	remove_variable(char *key, t_env **vars);
void	update_var(t_env *var, char *new_value ,char *key);
int		is_key_valid(char *key);
void	cd_error(char *path);
void	cmd_file_error(char *path, char *msg);
void	execve_error(char *cmd);
char	*get_key_value(char *key, t_env *env);
char	**sort_2d_array(char **arr);
char	**keys_to_2darray(t_env * env, int type);
int		ft_echo(t_cmd *cmd);
int		ft_pwd();
void	export_display(t_env **env);
void append_value(t_env **env, char *key, char *value);



#endif