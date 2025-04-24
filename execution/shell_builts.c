/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/24 20:10:09 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


void	ft_cd(t_cmd *cmd, t_malloc **aloc)
{
	struct stat st;
	char **arr;
	arr = extract_args(cmd->args);

	if(stat(arr[1], &st) != 0)
	{
		write(2,"sh: cd: ", 8);
		write(2, arr[1], ft_strlen(arr[1]));
		write(2,": No such file or directory\n", 28);
	}
	else
		chdir(arr[1]);
}

void	ft_export(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	char **arr;
	int i = 0;
	int check;
	char *value;

	arr = extract_args(cmd->name);
	while(arr[++i])
	{
		check = var_action(arr[i], env);
		if(check == 1)
		{
			
		}
		else if(check == 2)
		{
			
		}
		else if(check == 3)
		{
			
		}
	}
}

void	ft_env(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	
}

void	ft_exit(t_cmd *cmd, t_malloc **aloc)
{
	
}
