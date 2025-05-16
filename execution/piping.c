/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:11:23 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/16 09:45:33 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	count_cmd_list(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

int	**get_pipe_buff(t_cmd *cmd, t_malloc **alloc)
{
	int	n;
	int	**arr;

	n = 0;
	while (cmd)
	{
		cmd = cmd->next;
		n++;
	}
	arr = mmallocc(sizeof(int *) * (n - 1), alloc, P_GARBAGE);
	if (!arr)
		return (perror("malloc:"), NULL);
	while (--n > 0)
	{
		arr[n - 1] = mmallocc(2 * sizeof(int), alloc, P_GARBAGE);
		if (!arr[n - 1])
			return (perror("malloc:"), NULL);
		if (pipe(arr[n - 1]) == -1)
			return (perror("pipe:"), NULL);
	}
	return (arr);
}

void	close_fds(int pipe_count, int **arr)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (close(arr[i][0]) == -1 || close(arr[i][1]) == -1)
			perror("close:");
		i++;
	}
}

void	execute_pipeline(t_tools *tools)
{
	pid_t	pid;
	pid_t	right_most;
	int		num;
	int		status;
	int		**arr;
	int		cmd_count;

	cmd_count = count_cmd_list(tools->cmd);
	num = 0;
	right_most = -1;
	arr = get_pipe_buff(tools->cmd, tools->aloc);
	if (!arr)
	{
		*(tools->r_stat) = EXIT_FAILURE;
		return ;
	}
	while (tools->cmd)
	{
		pid = fork();
		if (pid == -1)
			perror("fork:");
		else if (pid == 0)
		{
			if (tools->cmd->next)
				if (dup2(arr[num][1], STDOUT_FILENO) == -1)
					return (printf("an error in fd[%i][1]\n", num), perror("dup2:"), exit(EXIT_FAILURE));
			if (num)
				if (dup2(arr[num - 1][0], STDIN_FILENO) == -1)
					return (printf("an error in fd[%i][0]\n", num), perror("dup2:"), exit(EXIT_FAILURE));
			close_fds(cmd_count - 1, arr);
			redirect_command(tools->cmd);
			execute_piped_cmd(tools);
			exit(*(tools->r_stat));
		}
		right_most = pid;
		tools->cmd = tools->cmd->next;
		num++;
	}
	close_fds(cmd_count - 1, arr);
	num = waitpid(right_most, &status, 0);
	if (num == -1)
	{
		if (errno == EINTR)
			*(tools->r_stat) = 130;
		else
			perror("waitpid pipe");
	}
	else if (num == right_most)
	{
		if (WIFEXITED(status))
			*(tools->r_stat) = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*(tools->r_stat) = 128 + WTERMSIG(status);		
	}
	while (waitpid(-1, &status, 0) > 0)
		;
}