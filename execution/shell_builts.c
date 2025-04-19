/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/19 12:33:17 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int ft_cd(t_cmd *cmd, t_malloc **aloc)
{
	char **args = extract_args(cmd->args);
	if(!access(cmd->name, X_OK))
	{
		if(execve(cmd->name, args, NULL) == -1)
			return(printf("execve failed in ft_cd"), -1);
	}
	else
	{
		char *path = get_executable_path("cd");
		if(path)
			if(execve(path, args, NULL) == -1)
				return(printf("execve failed in ft_cd"), -1);
	}
}

ft_export(t_cmd *cmd, t_malloc **aloc)
{
	0
}