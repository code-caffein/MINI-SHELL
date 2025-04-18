/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/18 18:08:28 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		printf("path checked = %s\n", check);
		if(!access(check, X_OK))
			return(check);
	}
	return(NULL);
}

int is_custom_builtins(char *str)
{
	if(!ft_strncmp("export", str, 13))
		return(1);
	else if(!ft_strncmp("unset", str, 13))
		return(2);
	else if(!ft_strncmp("exit", str, 13))
		return(3);
	else if(!ft_strncmp("cd", str, 13))
		return(4);
	return(0);
}

void execute_my_cmds(int i)
{
	if(i == 1)
		ft_export();
	else if(i == 2)
		ft_unset();
	else if(i == 3)
		ft_exit();
	else if(i == 4)
		ft_cd();
}

char **extract_args(t_arg *arg)
{
	t_arg *ptr;
	char **arr;
	int i;
	arr = malloc((sizeof(char*))*(count_list_size(arg, offsetof(t_arg, next))));
	i = 0;
	ptr = arg;
	while(ptr)
	{
		arr[i++] = ptr->value;
		ptr = ptr->next;
	}
	arr[i] = NULL;
	return(arr);
}

int ft_execute(t_cmd *cmds, t_malloc **allocs)
{
	pid_t pid;
	char *path;
	int i;
	int status;

	i = is_custom_builtins(cmds->name);
	if(i)
		return(execute_my_cmds(i), 1);
	pid = fork();
	if (pid > 0)
	{		
		waitpid(pid, &status, 0);
	}
	else
	{
		i = access(cmds->name, X_OK);
		if (i == -1)
		{
			path = get_executable_path(cmds->name);
			if (path)
				if (execve(path, cmds->args, NULL) == -1)
					return (printf("execve failed\n") ,-1);
		}
		else
			if(execve(cmds->name, cmds->args, NULL) == -1)
				return(printf("execve failed\n") ,-1);
	}
}

int main(void)
{
    t_cmd cmd;
    t_arg *arg1;
    t_malloc *allocs = NULL;

    // Simulate a command like: echo aymane
    arg1 = malloc(sizeof(t_arg));
    arg1->value = "aymane";
    arg1->need_expand = false;
    arg1->wait_more_args = false;
    arg1->type = text;

    t_arg *args[3];
    args[0] = malloc(sizeof(t_arg));
    args[0]->value = "echo";
    args[0]->need_expand = false;
    args[0]->wait_more_args = false;
    args[0]->type = text;

    args[1] = arg1;
    args[2] = NULL;

    cmd.name = "echo";
    cmd.args = args;
    cmd.arg_count = 2;
    cmd.arg_capacity = 3;
    cmd.syn_err = false;
    cmd.next = NULL;

    ft_execute(&cmd, &allocs);
    return (0);
}
