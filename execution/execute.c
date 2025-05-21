/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:36:58 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/21 16:35:49 by aelbour          ###   ########.fr       */
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

void	wait_for_child(t_tools *tools, int pid)
{
	int		status;
	int		sig;

	if (waitpid(pid, &status, 0) == -1)
	{
		critical_error("waitpid", tools->aloc, 0, tools->r_stat);
	}
	else if (WIFEXITED(status))
		*(tools->r_stat) = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		*(tools->r_stat) = 128 + sig;
	}
}

void	get_a_child(t_tools *tools)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		*(tools->r_stat) = 1;
		critical_error("fork", tools->aloc, 0, tools->r_stat);
		return ;
	}
	if (pid > 0)
		wait_for_child(tools, pid);
	else
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		tools->envp = vars_to_envp(tools);
		if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
			return (execve_error(tools), clean_up(tools->aloc));
	}
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


void exec_no_path_cmd_pipe(t_tools *tools)
{
	char	*path;

	path = get_executable_path(tools->cmd->name, tools->aloc, *(tools->env));
	if (path)
	{
		tools->envp = vars_to_envp(tools);
		tools->cmd->name = path;
		if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
			return (execve_error(tools), clean_up(tools->aloc), \
					exit(*(tools->r_stat)));
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tools->cmd->name, 2);
		ft_putstr_fd(": command not found\n", 2);
		*(tools->r_stat) = 127;
	}
}

void	execute_piped_cmd(t_tools *tools)
{
	int		i;

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
				return (execve_error(tools), clean_up(tools->aloc), \
						exit(*(tools->r_stat)));
	}
	else
		exec_no_path_cmd_pipe(tools);
}

void	fds_backup(int in_backup, int out_backup, t_malloc **aloc, int *r_stat)
{
	if (dup2(in_backup, 0) == -1 || dup2(out_backup, 1) == -1)
		critical_error("dup2", aloc, 0, r_stat);
	if (close(in_backup) == -1 || close(out_backup) == -1)
		critical_error("close", aloc, 0, r_stat);
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
				critical_error("dup", tools->aloc, 0, tools->r_stat);
			redirect_command(tools);
			ft_execute_simple_cmd(tools);
			fds_backup(in_backup, out_backup, tools->aloc, tools->r_stat);
		}
		else
			ft_execute_simple_cmd(tools);
	}
	g_signal_pid = a;
}
