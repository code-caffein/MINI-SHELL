/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:36:58 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/13 12:29:12 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

char	*get_executable_path(char *str, t_malloc **alloc)
{
	int		i;
	char	*s;
	char	**paths;
	char	*check;

	s = getenv("PATH");
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

void	get_a_child(int *g_exit_status, t_cmd *cmd, t_malloc **allocs, t_env **env, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		perror("fork:");
	if (pid > 0)
	{
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid:");
		else if (WIFEXITED(status))
			*g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*g_exit_status = 128 + WTERMSIG(status);
	}
	else
	{
		if (execve(cmd->name, cmd->args, envp) == -1)
			execve_error(cmd->name);
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

void	ft_execute_simple_cmd(t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status, char **envp)
{
	int		i;
	char	*path;

	if (!(cmd->name))
		return ;
	i = is_builtins(cmd->name);
	if (i)
		execute_builtin(i, cmd, allocs, env, g_exit_status);
	else if (ft_strchr(cmd->name, '/'))
	{
		if (file_error_handler(cmd->name, g_exit_status))
			get_a_child(g_exit_status ,cmd, allocs, env, envp);
	}
	else
	{
		path = get_executable_path(cmd->name, allocs);
		if (path)
		{
			cmd->name = path;
			get_a_child(g_exit_status, cmd, allocs, env, envp);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->name, 2);
			ft_putstr_fd(" : command not found\n", 2);
			*g_exit_status = 127;
		}
	}
}

void execute_piped_cmd(t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status, char **envp)
{
	int		i;
	char	*path;

	if (!cmd->name)
		exit(*g_exit_status);
	i = is_builtins(cmd->name);
	if (i)
		execute_builtin(i, cmd, allocs, env, g_exit_status);
	else if (ft_strchr(cmd->name, '/'))
	{
		if (file_error_handler(cmd->name, g_exit_status))
			if (execve(cmd->name, cmd->args, envp) == -1)
				execve_error(cmd->name);
	}
	else
	{
		path = get_executable_path(cmd->name, allocs);
		if (path)
		{
			cmd->name = path;
			if (execve(cmd->name, cmd->args, envp) == -1)
				execve_error(cmd->name);
		}
		else
		{
			printf("minishell: %s: command not found\n", cmd->name);
			*g_exit_status = 127;
		}
	}
}

void ft_execute(t_cmd *cmd, int *status, t_malloc **a, t_env **env, char **envp)
{
	int	in_backup;
	int	out_backup;

	if (cmd->next)
		execute_pipeline(cmd, a, env, status, envp);
	else
	{
		if (cmd->in || cmd->out)
		{
			in_backup = dup(STDIN_FILENO);
			out_backup = dup(STDOUT_FILENO);
			if (in_backup == -1 || out_backup == -1)
				perror("dup");
			redirect_command(cmd);
			ft_execute_simple_cmd(cmd, a, env, status, envp);
			if (dup2(in_backup, STDIN_FILENO) == -1
				|| dup2(out_backup, STDOUT_FILENO) == -1)
				perror("dup2:");
			if (close(in_backup) == -1 || close(out_backup) == -1)
				perror("close:");
		}
		else
			ft_execute_simple_cmd(cmd, a, env, status, envp);
	}
}
