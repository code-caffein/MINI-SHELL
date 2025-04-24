/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/24 16:03:21 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_executable_path(char *str)
{
	int  i;
	char *s = getenv("PATH");
	char **paths = ft_split(s,':');
	char *check;

	i = -1;
	while(paths[++i])
	{
		check = ft_strjoin(paths[i], ft_strjoin("/", str));
		printf("path checked = %s\n", check);
		if(!access(check, X_OK))
			return(check);
	}
	return(NULL);
}

int is_parent_builtins(char *str)
{
	int i = 0;
	char **arr;

	arr = ft_split(str, '/');
	while(arr[i + 1])
		i++;
	if(!ft_strcmp(arr[i], "export"))
		return(1);
	else if(!ft_strcmp(arr[i], "unset"))
		return(2);
	else if(!ft_strcmp(arr[i], "exit"))
		return(3);
	else if(!ft_strcmp(arr[i], "cd"))
		return(4);
	return(0);
}

void execute_parent_cmds(int i, t_cmd *cmd)
{
	if(i == 1)
		ft_export();
	else if(i == 2)
		ft_unset();
	else if(i == 3)
		ft_exit();
	else if(i == 4)
		ft_cd();
}

int get_a_child()
{

}

int ft_execute(t_cmd *cmd, t_malloc **allocs)
{
	pid_t pid;
	int  i;
	if(ft_strchr(cmd->name, '/'))
	{
		if(access(cmd->name, X_OK))
		{
			i = is_parent_builtins(cmd->name);
			if(i)
			{
				execute_parent_cmds(i, cmd);
			}
			else
			{
				
			}
		}
		else 
		{
			printf("command not found");	
		}
	}
	else
	{
		
	}
}
