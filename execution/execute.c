/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/17 15:55:57 by aelbour          ###   ########.fr       */
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

int ft_execute(t_cmd *cmds, t_malloc **allocs)
{
	int pid;
	char *path;
	int i;

	pid = fork;
	if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
	}
	else
	{
		i = access(cmds->name, X_OK);
		if (i == -1)
		{
			path = get_executable_path(cmds->name);
			if (path)
			{
				if (execve(path, cmds->args, NULL) == -1)
					return (printf("execve failed\n") ,-1);
			}
			else
			{
				printf("command not found !\n");
			}
		}
		else
		{
			if(execve(cmds->name, cmds->args, NULL) == -1)
				return(printf("execve failed\n") ,-1);
		}
	}
}