/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/21 11:24:46 by aelbour          ###   ########.fr       */
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
	else if(!ft_strncmp("env", str, 13))
		return(4);
	return(0);
}

void execute_my_cmds(int i, t_cmd *cmd)
{
	if(i == 1)
		ft_export();
	else if(i == 2)
		ft_unset();
	else if(i == 3)
		ft_exit();
	else if(i == 4)
		ft_cd();
	else if(i == 5)
		ft_env();
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
	char **args;

	i = is_custom_builtins(cmds->name);
	if (i)
		return (execute_my_cmds(i, cmds), 1);

	args = extract_args(cmds->args); // <== use your arg list to array here!

	pid = fork();
	if (pid < 0)
		return (perror("fork failed"), -1);

	if (pid > 0)
		waitpid(pid, &status, 0);
	else
	{
		i = access(cmds->name, X_OK);
		if (i == -1)
		{
			path = get_executable_path(cmds->name);
			if (path)
			{
				if (execve(path, args, NULL) == -1)
					return (perror("execve failed"), -1);
			}
			else
				return (printf("Command not found: %s\n", cmds->name), -1);
		}
		else
		{
			if (execve(cmds->name, args, NULL) == -1)
				return (perror("execve failed"), -1);
		}
	}
	return 0;
}


int main(void)
{
	t_cmd cmd;
	t_arg *arg1;
	t_arg *arg2;
	t_malloc *allocs = NULL;

	// Build first argument node (command name: echo)
	arg1 = malloc(sizeof(t_arg));
	arg1->value = "echo";
	arg1->need_expand = false;
	arg1->wait_more_args = false;
	arg1->type = text;

	// Second argument node (e.g., aymane)
	arg2 = malloc(sizeof(t_arg));
	arg2->value = "aymane";
	arg2->need_expand = false;
	arg2->wait_more_args = false;
	arg2->type = text;

	// Link the list
	arg1->next = arg2;
	arg2->next = NULL;

	// Fill command struct
	cmd.name = "echo";
	cmd.args = arg1;
	cmd.arg_count = 2;
	cmd.arg_capacity = 2;
	cmd.syn_err = false;
	cmd.next = NULL;

	ft_execute(&cmd, &allocs);
	return (0);
}

