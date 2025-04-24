/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/24 20:51:59 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


void	ft_cd(t_cmd *cmd, t_malloc **aloc)
{
	struct stat st;


	if(stat(cmd->args[1], &st) != 0)
	{
		write(2,"sh: cd: ", 8);
		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
		write(2,": No such file or directory\n", 28);
	}
	else
		chdir(cmd->args[1]);
}

void	ft_export(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	int i = 0;
	int check;
	char *value;


	while(cmd->args[++i])
	{
		check = var_action(cmd->args[i], *env);
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

void	ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	
}

void	ft_exit(t_cmd *cmd, t_malloc **aloc)
{
	
}

