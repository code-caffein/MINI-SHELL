/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:11:23 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/05 12:29:17 by aelbour          ###   ########.fr       */
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
	if(!arr)
		return(perror("malloc"), NULL);
	while(--n > 0)
	{
		arr[n - 1] = malloc(2* sizeof(int));
		if(!arr[n - 1])
			return(perror("malloc"), NULL);
		if(pipe(arr[n - 1]) == -1) 
			return(perror("pipe"), NULL);
	}
	return(arr);
}

void close_fds(int is_last, int num, int **arr, int is_parent)
{
	int i;

	i = 0;
	if(is_parent)
	{
		while(i < is_parent - 1)
		{
			close(arr[i][0]);
			close(arr[i][1]);
			i++;
		}
	}
	else if (!num)
		close(arr[num][0]); // close read end of pipe to next
	else if (is_last)
		close(arr[num - 1][1]); // close write end of previous
	else
	{
		close(arr[num - 1][1]); // write to previous
		close(arr[num][0]);     // read from next
	}
}

void execute_pipeline(t_cmd *cmd, t_malloc **a, t_env **env, int *last_status)
{
	pid_t pid;
	int num;
	int status;
	pid_t right_most;
	int **arr;

	num = 0;
	right_most  = -1;
	arr = get_pipe_buff(cmd);
	if(!arr)
		return(*last_status = EXIT_FAILURE);
	while(cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			close_fds(cmd->next == NULL, num, arr, 0);
			if (cmd->next)
			{
				if(dup2(arr[num][1], STDOUT_FILENO) == -1)
					return(perror("dup2"), exit(EXIT_FAILURE));
				close(arr[num][1]);
			}
			if (num)
			{
				if (dup2(arr[num - 1][0], STDIN_FILENO) == -1)
					return(perror("dup2"), exit(EXIT_FAILURE));
				close(arr[num - 1][0]);
			}
			execute_piped_cmd(cmd, a, env , last_status);
		}
		right_most = pid;
		cmd = cmd->next;
		num++;
	}
	close_fds(0, 0, arr, num);
	if(waitpid(right_most, &status, 0) == right_most)
	{
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*last_status = 128 + WTERMSIG(status);		
	}
	while (waitpid(-1 , &status, 0) > 0);
}


