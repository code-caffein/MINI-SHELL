/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:45:45 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/22 12:49:50 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_piped_cmd(t_tools *tools)
{
	int		i;

	if (!tools->cmd->name)
		exit(*(tools->r_stat));
	i = is_builtins(tools->cmd->name);
	if (i)
		execute_builtin(i, tools);
	else if (ft_strchr(tools->cmd->name, '/'))
	{
		tools->envp = vars_to_envp(tools);
		if (file_error_handler(tools->cmd->name, tools->r_stat))
			if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
				return (execve_error(tools), clean_up(tools->aloc), \
						exit(*(tools->r_stat)));
	}
	else
		exec_no_path_cmd_pipe(tools);
}

void	exec_no_path_cmd_pipe(t_tools *tools)
{
	char	*path;

	path = get_executable_path(tools->cmd->name, tools->aloc, *(tools->env));
	if (path)
	{
		tools->envp = vars_to_envp(tools);
		tools->cmd->name = path;
		if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
			return (execve_error(tools), clean_up(tools->aloc), \
					exit(*(tools->r_stat)));
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tools->cmd->name, 2);
		ft_putstr_fd(": command not found\n", 2);
		*(tools->r_stat) = 127;
	}
}

void	close_fds(int pipe_count, int **arr, t_tools *tools)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		if (close(arr[i][0]) == -1 || close(arr[i][1]) == -1)
			critical_error("close", tools->aloc, 0, tools->r_stat);
		i++;
	}
}

int	**get_pipe_buff(t_tools *tools, int count)
{
	int	n;
	int	**arr;

	n = count;
	arr = mmallocc(sizeof(int *) * (n - 1), tools->aloc, P_GARBAGE);
	while (--n > 0)
	{
		arr[n - 1] = mmallocc(2 * sizeof(int), tools->aloc, P_GARBAGE);
		if (pipe(arr[n - 1]) == -1)
			return (critical_error("pipe", tools->aloc, 0, \
					tools->r_stat), NULL);
	}
	return (arr);
}
