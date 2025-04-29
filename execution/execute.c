/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/29 10:04:14 by aelbour          ###   ########.fr       */
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

int is_builtins(char *str)
{
	int i = 0;
	char **arr;

	if(!ft_strcmp(arr[i], "echo"))
		return(1);
	else if(!ft_strcmp(arr[i], "cd"))
		return(2);
	else if(!ft_strcmp(arr[i], "pwd"))
		return(3);
	else if(!ft_strcmp(arr[i], "export"))
		return(4);
	else if(!ft_strcmp(arr[i], "unset"))
		return(5);
	else if(!ft_strcmp(arr[i], "env"))
		return(6);
	else if(!ft_strcmp(arr[i], "exit"))
		return(7);
	return(0);
}

void	execute_parent_cmds(int i, t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{
	if(i == 1)
		*g_exit_status = ft_echo(cmd);
	else if(i == 2)
		*g_exit_status = ft_cd(cmd, allocs);
	else if(i == 3)
		*g_exit_status = ft_pwd(cmd, allocs);
	else if(i == 4)
		*g_exit_status = ft_export(cmd, allocs, env);
	else if(i == 5)
		*g_exit_status = ft_unset(cmd, allocs, env);
	else if(i == 6)
		ft_env(allocs, env);
	else if(i == 7)
		ft_exit(allocs, *g_exit_status);
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

void ft_execute(t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{
	int		i;
	char	*path;

	i = is_builtins(cmd->name);
	if (i)
		execute_parent_cmds(i, cmd, allocs, env, g_exit_status);
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
			if (ft_strcmp("env", cmd->name))
				cmd->name = path;
			cmd->args[0] = path;
			get_a_child(g_exit_status, cmd, allocs, env);
		}
		else
		{
			printf("minishell: %s: command not found\n", cmd->name);
			*g_exit_status = 127;
		}
	}
}

int main(int ac, char **av, char **envp)
{
	t_cmd		cmd;
	t_env		*env;
	t_malloc	*alloc;
	int			status;

	env = NULL;
	cmd.args=ft_split("export elbour", ' ');
	ft_export(&cmd, NULL, &env);

	cmd.args=ft_split("export elbour+=hello ", ' ');
	ft_export(&cmd, NULL, &env);
	
	cmd.args=ft_split("export", ' ');
	ft_export(&cmd, NULL, &env);
}
