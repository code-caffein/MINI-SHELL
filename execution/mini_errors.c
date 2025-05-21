/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:21:50 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/21 14:51:28 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	cd_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd("\n", 2);
}

void	cmd_file_error(char *path, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

void	execve_error(t_tools *tools)
{
	ft_putstr_fd("minishell: ", 21);
	ft_putstr_fd(tools->cmd->name, 2);
	if (errno == ENOENT)
	{
		ft_putstr_fd(": command not found\n", 2);
		*(tools->r_stat) = 127;
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd(": Permission denied\n", 2);
		*(tools->r_stat) = 126;
	}
	else
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		*(tools->r_stat) = 1;
	}
}

int	file_error_handler(char *path, int *status)
{
	struct stat	info;

	if (stat(path, &info) == 0)
	{
		if (S_ISDIR(info.st_mode) == true)
			cmd_file_error(path, "is a Directorie");
		else if (access(path, X_OK) == 0)
			return (1);
		else
			cmd_file_error(path, "permission denied");
		*status = 126;
	}
	else
	{
		cmd_file_error(path, "No such file or directory");
		*status = 127;
	}
	return (0);
}