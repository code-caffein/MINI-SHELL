/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:24:27 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/21 15:39:32 by aelbour          ###   ########.fr       */
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
