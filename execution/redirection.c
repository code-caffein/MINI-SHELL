/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:31:09 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/21 16:08:31 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
