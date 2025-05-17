/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:22:11 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/17 09:13:36 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	is_builtins(char *str)
{
	int	i;

	i = 0;
	if (!ft_strcmp(str, "echo"))
		return (1);
	else if (!ft_strcmp(str, "cd"))
		return (2);
	else if (!ft_strcmp(str, "pwd"))
		return (3);
	else if (!ft_strcmp(str, "export"))
		return (4);
	else if (!ft_strcmp(str, "unset"))
		return (5);
	else if (!ft_strcmp(str, "env"))
		return (6);
	else if (!ft_strcmp(str, "exit"))
		return (7);
	return (0);
}

void	execute_builtin(int i, t_tools *tools)
{
	if (i == 1)
		*(tools->r_stat) = ft_echo(tools->cmd);
	else if (i == 2)
		*(tools->r_stat) = ft_cd(tools);
	else if (i == 3)
		*(tools->r_stat) = ft_pwd(tools->env, NULL, NULL, tools);
	else if (i == 4)
		*(tools->r_stat) = ft_export(tools);
	else if (i == 5)
		*(tools->r_stat) = ft_unset(tools->cmd, tools->aloc, tools->env);
	else if (i == 6)
		*(tools->r_stat) = ft_env(tools->aloc, tools->env, tools->cmd);
	else if (i == 7) 
		ft_exit(tools->aloc, tools->cmd, (tools->r_stat));
}
