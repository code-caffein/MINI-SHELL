/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/21 11:37:05 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


void	ft_cd(t_cmd *cmd, t_malloc **aloc, t_env **env)
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

	arr = extract_args(cmd->name);
	
}

void	ft_export(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	
}

void	ft_env(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	
}

void	ft_exit(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	
}
