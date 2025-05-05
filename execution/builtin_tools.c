/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:22:11 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/05 10:23:10 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int is_builtins(char *str)
{
	int i = 0;

	if(!ft_strcmp(str, "echo"))
		return(1);
	else if(!ft_strcmp(str, "cd"))
		return(2);
	else if(!ft_strcmp(str, "pwd"))
		return(3);
	else if(!ft_strcmp(str, "export"))
		return(4);
	else if(!ft_strcmp(str, "unset"))
		return(5);
	else if(!ft_strcmp(str, "env"))
		return(6);
	else if(!ft_strcmp(str, "exit"))
		return(7);
	return(0);
}

void	execute_builtin(int i, t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{
	if(i == 1)
		*g_exit_status = ft_echo(cmd);
	else if(i == 2)
		*g_exit_status = ft_cd(cmd, allocs);
	else if(i == 3)
		*g_exit_status = ft_pwd(cmd, allocs);
	else if(i == 4)
		*g_exit_status = ft_export(cmd, allocs, env);
	else if(i == 5)
		*g_exit_status = ft_unset(cmd, allocs, env);
	else if(i == 6)
		*g_exit_status = ft_env(allocs, env, cmd);
	else if(i == 7)
		ft_exit(allocs, cmd, g_exit_status);
}
