/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:21:50 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/27 15:03:58 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void cd_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd("\n", 2);
}

void cmd_file_error(char *path, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

void execve_error(char *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	if (errno == ENOENT)
	{
		write(2, ": command not found\n", 21);
		exit(127);
	}
	else if (errno == EACCES)
	{
		write(2, ": Permission denied\n", 21);
		exit(126);
	}
	else
	{
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		exit(1);
	}
}
