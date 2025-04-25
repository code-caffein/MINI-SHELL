#ifndef EXECUTION_H
#define EXECUTION_H


#include<stdio.h> 
#include<unistd.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "../includes/minishell.h"

char	*get_executable_path(char *str);
void	ft_export(t_cmd *cmd, t_malloc **aloc, t_env **env);
void	ft_cd(t_cmd *cmd, t_malloc **aloc);
void	ft_env(t_cmd *cmd, t_malloc **aloc, t_env **env);
void	ft_exit(t_cmd *cmd, t_malloc **aloc);
void	ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env);
int var_action(char *s, t_env *env);
t_env	*get_bef_node(t_env *lst, t_env *node);
int is_var_exist(char *key, t_env *vars);
void remove_variable(char *key, t_env **vars);
void *push_to_env(t_env **head, char *key, char *value);



#endif