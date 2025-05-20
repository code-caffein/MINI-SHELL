/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:36:58 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/20 11:34:37 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	g_signal_pid;

char	*get_executable_path(char *str, t_malloc **alloc, t_env *env)
{
	int		i;
	char	*s;
	char	**paths;
	char	*check;

	if (!str || !str[0])
		return (NULL);
	s = get_key_value("PATH", env);
	paths = ft_split(s, ':', alloc);
	i = -1;
	while (paths[++i])
	{
		check = ft_strjoin(paths[i], ft_strjoin("/", str, alloc), alloc);
		if (!access(check, X_OK))
			return (check);
	}
	return (NULL);
}
		// printf("path checked = %s\n", check);

void	get_a_child(t_tools *tools)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid > 0)
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
		}
		else if (WIFEXITED(status))
			*(tools->r_stat) = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(2, "Quit: 3\n", 8);
			*(tools->r_stat) = 128 + sig;
		}
	}
	else
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		tools->envp = vars_to_envp(tools);
		if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
			execve_error(tools->cmd->name);
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

void	ft_execute_simple_cmd(t_tools *tools)
{
	int		i;
	char	*path;

	if (!(tools->cmd->name))
		return ;
	i = is_builtins(tools->cmd->name);
	if (i)
		execute_builtin(i, tools);
	else if (ft_strchr(tools->cmd->name, '/'))
	{
		if (file_error_handler(tools->cmd->name, (tools->r_stat)))
			get_a_child(tools);
	}
	else
	{
		path = get_executable_path(tools->cmd->name, tools->aloc, *(tools->env));
		if (path)
		{
			tools->cmd->name = path;
			get_a_child(tools);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(tools->cmd->name, 2);
			ft_putstr_fd(": command not found\n", 2);
			*(tools->r_stat) = 127;
		}
	}
}

void	execute_piped_cmd(t_tools *tools)
{
	int		i;
	char	*path;

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
				execve_error(tools->cmd->name);
	}
	else
	{
		path = get_executable_path(tools->cmd->name, tools->aloc, *(tools->env));
		if (path)
		{
			tools->envp = vars_to_envp(tools);
			tools->cmd->name = path;
			if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
				execve_error(tools->cmd->name);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(tools->cmd->name, 2);
			ft_putstr_fd(": command not found\n", 2);
			*(tools->r_stat) = 127;
		}
	}
}

void	fds_backup(int in_backup, int out_backup)
{
	if (dup2(in_backup, 0) == -1 || dup2(out_backup, 1) == -1)
		perror("dup2:");
	if (close(in_backup) == -1 || close(out_backup) == -1)
		perror("close:");
}

void	ft_execute(t_tools *tools)
{
	int	a;
	int	in_backup;
	int	out_backup;

	a = g_signal_pid;
	g_signal_pid = 4;
	if (tools->cmd->next)
		execute_pipeline(tools);
	else
	{
		if (tools->cmd->in || tools->cmd->out)
		{
			in_backup = dup(STDIN_FILENO);
			out_backup = dup(STDOUT_FILENO);
			if (in_backup == -1 || out_backup == -1)
				perror("dup");
			redirect_command(tools->cmd);
			ft_execute_simple_cmd(tools);
			fds_backup(in_backup, out_backup);
		}
		else
			ft_execute_simple_cmd(tools);
	}
	g_signal_pid = a;
}
