/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:11:23 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/07 14:54:44 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


int count_cmd_list(t_cmd *cmd)
{
	int i;

	i = 0;
	while(cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}
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

void close_fds(int pipe_count, int **arr)
{
    int i = 0;
    while (i < pipe_count)
    {
        // if (arr[i][0] != -1)
            close(arr[i][0]);
        // if (arr[i][1] != -1)
            close(arr[i][1]);
        i++;
    }
}


void execute_pipeline(t_cmd *cmd, t_malloc **a, t_env **env, int *last_status)
{
	pid_t pid;
	int num;
	int status;
	pid_t right_most;
	int **arr;
	int cmd_count;

	cmd_count = count_cmd_list(cmd);
	num = 0;
	right_most  = -1;
	arr = get_pipe_buff(cmd);
	if(!arr)
	{
		*last_status = EXIT_FAILURE;
		return;
	}
	while(cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			if (cmd->next)
				if (dup2(arr[num][1], STDOUT_FILENO) == -1)
					return (printf("an error in fd[%i][1]\n", num),perror("dup2 write"), exit(EXIT_FAILURE));
			if (num)
				if (dup2(arr[num - 1][0], STDIN_FILENO) == -1)
					return (printf("an error in fd[%i][0]\n", num),perror("dup2 read"), exit(EXIT_FAILURE));
			close_fds(cmd_count - 1, arr);
			redirect_command(cmd);
			execute_piped_cmd(cmd, a, env , last_status);
			exit(*last_status);
		}
		right_most = pid;
		cmd = cmd->next;
		num++;
}


