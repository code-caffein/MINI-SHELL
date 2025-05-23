/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:11:23 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/23 16:07:50 by aelbour          ###   ########.fr       */
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

void	piped_child(t_tools *tools, int cmd_count, int **arr, int num)
{
	if (tools->cmd->next)
		if (dup2(arr[num][1], STDOUT_FILENO) == -1)
			return (critical_error("dup2", tools->aloc, 1, NULL));
	if (num)
		if (dup2(arr[num - 1][0], STDIN_FILENO) == -1)
			return (critical_error("dup2", tools->aloc, 1, NULL));
	close_fds(cmd_count - 1, arr, tools);
	redirect_command(tools);
	execute_piped_cmd(tools);
	exit(*(tools->r_stat));
}

int	manage_pipes_redirection(t_tools *tools, int cmd_count, \
	int **arr, pid_t pid)
{
	int	num;

	num = 0;
	cmd_count = count_cmd_list(tools->cmd);
	arr = get_pipe_buff(tools, cmd_count);
	while (arr && tools->cmd)
	{
		pid = fork();
		if (pid == -1)
		{
			*(tools->r_stat) = 1;
			critical_error("fork", tools->aloc, 0, tools->r_stat);
		}
		else if (pid == 0)
			piped_child(tools, cmd_count, arr, num);
		tools->cmd = tools->cmd->next;
		num++;
	}
	close_fds(cmd_count - 1, arr, tools);
	return (pid);
}

void	execute_pipeline(t_tools *tools)
{
	int		status;
	int		num;
	int		right_most;

	right_most = manage_pipes_redirection(tools, 0, NULL, -1);
	if (right_most == -1)
		return ;
	num = waitpid(right_most, &status, 0);
	if (num == -1)
		critical_error("waitpid", tools->aloc, 0, tools->r_stat);
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
