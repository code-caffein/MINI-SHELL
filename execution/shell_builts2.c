/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:14:18 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/23 10:16:49 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exit_numeric_error(char *arg ,t_malloc **aloc)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	clean_up(aloc);
	exit(255);
}

void	failed_exit_args_num(int *status)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	*status = 1;
}

int	handle_cd_failed_cwd(t_env **env, t_tools *tools, \
		t_malloc **a, char *to_path)
{
	char	*oldpwd;

	ft_putstr_fd("cd: error retrieving current directory: ", 2);
	perror("getcwd: cannot access parent directories");
	oldpwd = get_key_value("PWD", *env);
	if (oldpwd && ft_strlen(oldpwd))
	{
		update_var(tools, ft_strdup(oldpwd, a, P_ENV), "OLDPWD");
		update_var(tools, ft_strdup(ft_strjoin(ft_strjoin(oldpwd, "/", a) \
		, to_path, a), a, P_ENV), "p.a.t.h");
		update_var(tools, ft_strdup(ft_strjoin(ft_strjoin(oldpwd, "/", a) \
		, to_path, a), a, P_ENV), "PWD");
	}
	else if (oldpwd)
	{
		update_var(tools, ft_strdup(oldpwd, a, P_ENV), "OLDPWD");
		update_var(tools, ft_strdup(ft_strjoin(oldpwd, to_path, a) \
		, a, P_ENV), "p.a.t.h");
		update_var(tools, ft_strdup(ft_strjoin(oldpwd, to_path, a) \
		, a, P_ENV), "PWD");
	}
	return (0);
}

int	handle_home(t_tools *tools)
{
	char		*home;

	home = get_key_value("HOME", *(tools->env));
	if (!home)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	if (chdir(home) == -1)
	{
		cd_error(tools->cmd->args[1]);
		return (1);
	}
	else
	{
		ft_pwd(tools->env, tools->aloc, tools->cmd->args[1], tools);
	}
	return (0);
}
