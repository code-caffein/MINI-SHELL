/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:24:27 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/23 16:03:44 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	critical_error(char *syscall, t_malloc **alloc, int quit, int *r_stat)
{
	perror(syscall);
	if (quit)
	{
		if (strcmp(syscall, "malloc"))
			clean_up(alloc);
		exit(EXIT_FAILURE);
	}
	else
		*r_stat = 1;
}

void clean_files(t_tools *tools)
{
	t_redirection *cursor;

	if (tools && tools->cmd)
	{
		cursor = tools->cmd->in;
		while (cursor)
		{
			if (cursor->fd > 0)
				close(cursor->fd);
			cursor = cursor->next;
		}

		cursor = tools->cmd->out;
		while (cursor)
		{
			if (cursor->fd > 0)
				close(cursor->fd);
			cursor = cursor->next;
		}
	}
	}

