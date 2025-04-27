/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/26 19:42:40 by aelbour          ###   ########.fr       */
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

int is_parent_builtins(char *str)
{
	int i = 0;
	char **arr;

	arr = ft_split(str, '/');
	while(arr[i + 1])
		i++;
	if(!ft_strcmp(arr[i], "export"))
		return(1);
	else if(!ft_strcmp(arr[i], "unset"))
		return(2);
	else if(!ft_strcmp(arr[i], "exit"))
		return(3);
	else if(!ft_strcmp(arr[i], "cd"))
		return(4);
	return(0);
}

int execute_parent_cmds(int i, t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{

	if(i == 1)
		*g_exit_status=ft_export(cmd, allocs, env);
	else if(i == 2)
		*g_exit_status=ft_unset(cmd, allocs, env);
	else if(i == 3)
		ft_exit(allocs, *g_exit_status);
	else if(i == 4)
		*g_exit_status=ft_cd(cmd, allocs);
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
		if(ft_strcmp("env", cmd->name))
		{
			ft_env(allocs, env);
			exit(0);
		}
		else if (execve(cmd->name, cmd->args, NULL) == -1)
			printf("%s: permission denied\n", cmd->name);
	}
}

int file_error_handler(char *path)
{
	struct stat info;

	if(stat(path, &info) == 0)
	{ 
		if(S_ISDIR(info.st_mode) == true)
			printf("%s: is a Directorie\n", path);
		else if(access(path, X_OK) == 0)
			return(1);
		else
			printf("%s: permission denied\n", path);
	}
	else
		printf("%s: No such file or directory\n", path);
	return(0);
}

int ft_execute(t_cmd *cmd, t_malloc **allocs, t_env *env)
{
	int		i;
	char	*path;
	int g_exit_status;

	i = is_parent_builtins(cmd->name);
	if(i)
		g_exit_status = execute_parent_cmds(i, cmd, allocs, &env, g_exit_status);
	else if(ft_strchr(cmd->name, '/'))
	{
		if(file_error_handler(cmd->name))
			get_a_child(&g_exit_status ,cmd, allocs, &env);
	}
	else
	{
		path = get_executable_path(cmd->name);
		if (path)
		{
			cmd->name = path;
			cmd->args[0] = path;
			get_a_child(&g_exit_status, cmd, allocs, &env);
		}
		else
		{
			printf("%s: command not found\n", cmd->name);
		}
	}
	return(1);
}

int main(void)
{
	t_env *env = NULL;      // your environment linked list
	t_malloc *allocs = NULL; // your malloc tracker
	t_cmd cmd;               // fake command struct
	char *args1[] = {"export", "a=hello", "b=world", NULL};
	char *args2[] = {"export", "a", NULL};
	char *args3[] = {"export", NULL};
	char *args4[] = {"export", "aymane=", NULL};
	char *args5[] = {"export", "elbour", NULL};

	// First command: export a=hello b=world
	cmd.args = args1;
	ft_export(&cmd, &allocs, &env);
	printf("\n");

	// Second command: export a (no assignment)
	cmd.args = args2;
	ft_export(&cmd, &allocs, &env);
	printf("\n");

	cmd.args = args4;
	ft_export(&cmd, &allocs, &env);
	printf("\n");

	cmd.args = args5;
	ft_export(&cmd, &allocs, &env);
	printf("\n");
	
	// Third command: export (no args, should print all)
	cmd.args = args3;
	ft_export(&cmd, &allocs, &env);
	printf("\n");

	// Bonus: you can add more tests here if you want
}