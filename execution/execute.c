/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:36:58 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/07 10:37:18 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

char *get_executable_path(char *str)
{
	int  i;
	char *s = getenv("PATH");
	char **paths = ft_split(s,':');
	char *check;

	i = -1;
	while(paths[++i])
	{
		check = ft_strjoin(paths[i], ft_strjoin("/", str));
		// printf("path checked = %s\n", check);
		if(!access(check, X_OK))
			return(check);
	}
	return(NULL);
}

void get_a_child(int *g_exit_status, t_cmd *cmd, t_malloc **allocs, t_env **env)
{
	pid_t pid;
	int status;

	pid = fork();
	if(pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*g_exit_status = 128 + WTERMSIG(status);
	}
	else
	{
		if (execve(cmd->name, cmd->args, NULL) == -1)
			execve_error(cmd->name);
	}
}

int file_error_handler(char *path, int *status)
{
	struct stat	info;

	if (stat(path, &info) == 0)
	{
		if (S_ISDIR(info.st_mode) == true)
			cmd_file_error(path,"is a Directorie");
		else if (access(path, X_OK) == 0)
			return (1);
		else
			cmd_file_error(path,"permission denied");
		*status = 126;
	}
	else
	{
		cmd_file_error(path,"No such file or directory");
		*status = 127;
	}
	return (0);
}

void ft_execute_simple_cmd(t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{
	int		i;
	char	*path;

	i = is_builtins(cmd->name);
	if (i)
		execute_builtin(i, cmd, allocs, env, g_exit_status);
	else if (ft_strchr(cmd->name, '/'))
	{
		if (file_error_handler(cmd->name, g_exit_status))
			get_a_child(g_exit_status ,cmd, allocs, env);
	}
	else
	{
		path = get_executable_path(cmd->name);
		// printf("executable path = %s\n", path);
		if (path)
		{
			cmd->name = path;
			get_a_child(g_exit_status, cmd, allocs, env);
		}
		else
		{
			printf("minishell: %s: command not found\n", cmd->name);
			*g_exit_status = 127;
		}
	}
}

void execute_piped_cmd(t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{
	int		i;
	char	*path;

	i = is_builtins(cmd->name);
	if (i)
		execute_builtin(i, cmd, allocs, env, g_exit_status);
	else if (ft_strchr(cmd->name, '/'))
	{
		if (file_error_handler(cmd->name, g_exit_status))
			if (execve(cmd->name, cmd->args, NULL) == -1)
				execve_error(cmd->name);
	}
	else
	{
		path = get_executable_path(cmd->name);
		// printf("executable path = %s\n", path);
		if (path)
		{
			cmd->name = path;
			if (execve(cmd->name, cmd->args, NULL) == -1)
				execve_error(cmd->name);
		}
		else
		{
			printf("minishell: %s: command not found\n", cmd->name);
			*g_exit_status = 127;
		}
	}
}

void ft_execute(t_cmd *cmd, int *status, t_malloc **a, t_env **env)
{
	int in_backup;
	int out_backup;

	
	if(cmd->next)
		execute_pipeline(cmd, a, env , status);
	else
	{
		if(cmd->in || cmd->out)
		{
			in_backup = dup(STDIN_FILENO);
			out_backup = dup(STDOUT_FILENO);
			redirect_command(cmd);
			ft_execute_simple_cmd(cmd, a, env , status);
			dup2(in_backup, STDIN_FILENO);
			dup2(out_backup, STDOUT_FILENO);
			close(in_backup);
			close(out_backup);
		}
		else
			ft_execute_simple_cmd(cmd, a, env , status);
	}
}
