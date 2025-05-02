/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:11:23 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/02 16:54:30 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int **get_pipe_buff(t_cmd *cmd)
{
	int n;
	int **arr;
	n = 0;
	while(cmd)
	{
		cmd = cmd->next;
		n++;
	}
	arr = malloc(sizeof(int *) * (n - 1));
	while(--n > 0)
	{
		arr[n - 1] = malloc(2* sizeof(int));
		pipe(arr[n - 1]);
	}
	return(arr);
}

void close_fds(void *p, int num, int **arr)
{
	int i;

	if(num)
		close(arr[num][0]);
	else if(!p)
		close(arr[num - 1][1]);
	else
	{
		close(arr[num - 1][0]);
		close(arr[num - 1][1]);
		
	}
}

void execute_piped_cmd(t_cmd *cmd)
{
	int pid;
	int num;
	int status;
	int **arr;

	num = 0;
	arr = get_pipe_buff(cmd);
	while(cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			if(cmd->next)
				dup2(arr[num][1], STDOUT_FILENO);
			else if(num)
				dup2(arr[num - 1][0], STDIN_FILENO);
		}
		cmd = cmd->next;
		num++;
	}
	waitpid(pid, &status, 0);
	while (wait(NULL) ==  -1);
}

void execute_cmd_line(t_cmd *cmd)
{
	int num;

	if(cmd->next)
		execute_piped_cmd(cmd);
	else
		ft_execute_simple_cmd(cmd);
}