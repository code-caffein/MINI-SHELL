/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:31:09 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/06 15:51:47 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


void output_redirection(t_cmd *cmd)
{
	t_redirection *cur;
	cur = cmd->out;
	
	while(cur->next)
	{
		// if(cur->type == REDIR_APPEND)
		cur = cur->next;
	}
	dup2(cur->fd, STDOUT_FILENO);
}

void input_redirection(t_cmd *cmd)
{
	t_redirection *cur;
	cur = cmd->in;
	
	while(cur->next)
	{
		// if(cur->type == REDIR_APPEND)
		cur = cur->next;
	}
	dup2(cur->fd, STDIN_FILENO);
}

void redirect_command(t_cmd *cmd)
{
	if(cmd -> out)
	{
		output_redirection(cmd);
	}
	if(cmd -> in)
	{
		input_redirection(cmd);
	}
}