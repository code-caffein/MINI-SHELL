/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:31:09 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/22 12:51:01 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	fds_backup(int in_backup, int out_backup, t_malloc **aloc, int *r_stat)
{
	if (dup2(in_backup, 0) == -1 || dup2(out_backup, 1) == -1)
		critical_error("dup2", aloc, 0, r_stat);
	if (close(in_backup) == -1 || close(out_backup) == -1)
		critical_error("close", aloc, 0, r_stat);
}

void	output_redirection(t_tools *tools)
{
	t_redirection	*cur;

	cur = tools->cmd->out;
	while (cur->next)
	{
		cur = cur->next;
	}
	if (dup2(cur->fd, STDOUT_FILENO) == -1)
		critical_error("dup2", tools->aloc, 0, tools->r_stat);
}

void	input_redirection(t_tools *tools)
{
	t_redirection	*cur;

	cur = tools->cmd->in;
	while (cur->next)
	{
		cur = cur->next;
	}
	if (dup2(cur->fd, STDIN_FILENO) == -1)
		critical_error("dup2", tools->aloc, 0, tools->r_stat);
}

void	redirect_command(t_tools *tools)
{
	if (tools->cmd->out)
	{
		output_redirection(tools);
	}
	if (tools->cmd->in)
	{
		input_redirection(tools);
	}
}
