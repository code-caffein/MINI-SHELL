/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/26 21:02:34 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_cd(t_cmd *cmd, t_malloc **aloc)
{
	struct stat st;

	if(cmd->args[1] == NULL)
		return(1);
	else if(stat(cmd->args[1], &st) != 0)
	{
		if(errno == ENOENT)
			printf("cd: no such file or directory: %s\n", cmd->args[1]);
		if(errno == EACCES)
			printf("cd: permission denied: %s\n", cmd->args[1]);
		return(1);
	}
	else
	{
		if(S_ISDIR(st.st_mode))
		{
			if(chdir(cmd->args[1]) == -1);
				
		}
		else
		{
			printf("cd: not a directory: %s\n", cmd->args[1]);
			return(1);
		}
	}
	return(0);
}

int	ft_export(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	int i = 0;
	int check;
	int status;
	char *value;
	char *key;
	t_env *ptr = *env; 

	status = 0;
 	while(cmd->args[++i])
	{
		key = ft_split(cmd->args[i], '=')[0];
		value = ft_strchr(cmd->args[i], '=');
		if(value)
			value++;
		check = var_action(key, value, *env);
		if(check == 1)
			push_to_env(env, key, value);
		else if(check == 2)
			update_var(*env, value, key);
		else if(check == 3)
		{
			status = 1;
	 		printf("minishell: export: `%s': not a valid identifier\n", key);
		}
	}
	if(i == 1)
	{
		while(ptr)
		{
			if(ptr->value == NULL)
				printf("declare -x %s\n", ptr->key);
			else
				printf("declare -x %s=\"%s\"\n", ptr->key, ptr->value);
			ptr=ptr->next;
		}
	}
	return(status);
}

void	ft_env(t_malloc **aloc, t_env **env)
{
	t_env *ptr = *env; 

	while(ptr)
	{
		if(ptr->value)
			printf("%s=%s\n", ptr->key, ptr->value);
		ptr=ptr->next;
	}
}

int	ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	int i = 0;
	int status= 0;
	t_env *ptr = *env; 

 	while(cmd->args[++i])
	{
		if(is_var_exist(cmd->args[i], *env))
			remove_variable(cmd->args[i], env);
		if(!is_key_valid(cmd->args[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n", cmd->name[i]);
			status = 1;
		}
	}
	return(status);
}

void	ft_exit(t_malloc **aloc, int status)
{
	clean_up(malloc);
	exit(status);
}

