/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/24 21:08:27 by aelbour          ###   ########.fr       */
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

void execute_parent_cmds(int i, t_cmd *cmd, t_malloc **allocs, t_env **env)
{
	if(i == 1)
		ft_export(cmd, allocs, env);
	else if(i == 2)
		ft_unset(cmd, allocs, env);
	else if(i == 3)
		ft_exit(cmd, allocs);
	else if(i == 4)
		ft_cd(cmd, allocs);
}

void get_a_child(t_cmd *cmd, t_malloc **allocs)
{
	pid_t pid;
	int i;

	pid = fork();
	if(pid > 0)
		waitpid(pid, &i, 0);
	else
	{
		if (execve(cmd->name, cmd->args, NULL) == -1)
			printf("cant execute it\n");
	}
}

void file_error_handler(char *path)
{
	struct stat info;

	if(stat(path, &info) == 0)
	{
		if(S_ISDIR(info.st_mode) == true)
			printf("%s: is a Directorie\n", path);
		else
			printf("%s: permission denied\n", path);
	}
	else
		printf("command not found\n");
}

int ft_execute(t_cmd *cmd, t_malloc **allocs, t_env *env)
{
	int		i;
	char	*path;
	i = is_parent_builtins(cmd->name);
	if(i)
	execute_parent_cmds(i, cmd, allocs, &env);
	if(ft_strchr(cmd->name, '/'))
	{
		if(access(cmd->name, X_OK) == 0)
		{
			get_a_child(cmd, allocs);
		}
		else 
		{
			file_error_handler(cmd->name);
			exit(1);
		}
	}
	else
	{
		path = get_executable_path(cmd->name);
		if (path)
			if (execve(path, cmd->args, NULL) == -1)
				printf("cant execute it\n");
		else
			printf("command not found\n");
	}
}

int main(int argc, char **argv, char **envp)
{
	t_cmd		cmd;
	t_malloc	*allocs = NULL;
	t_env *env;

	(void)argc;
	(void)envp;

	// Example command: ls -l
	cmd.name = "/usr/bin";
	cmd.args = malloc(sizeof(char *) * 3);
	cmd.args[0] = "/usr/bin";
	cmd.args[1] = "-l";
	cmd.args[2] = NULL;

	ft_execute(&cmd, &allocs, env);

	return (0);
}
