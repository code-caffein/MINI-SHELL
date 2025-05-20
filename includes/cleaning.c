/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:24:27 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/20 11:46:24 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_files(t_cmd *cmd)
{
	t_redirection	*cursor;

	if (cmd->in)
	{
		cursor = cmd->in;
		while (cursor)
		{
			if (close(cursor->fd) == -1)
				perror("minishell: close:");
			cursor = cursor->next;
		}
	}
	if (cmd->out)
	{
		cursor = cmd->out;
		while (cursor)
		{
			if ((cursor->fd) == -1)
				perror("minishell: close:");
			cursor = cursor->next;
		}
	}
}

void	critical_error(char *syscall, t_malloc **alloc)
{
	perror(syscall);
	clean_up(alloc);
	exit(EXIT_FAILURE);
}
