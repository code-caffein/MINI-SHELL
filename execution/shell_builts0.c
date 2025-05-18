/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/18 12:27:44 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_cd(t_tools *tools)
{
	struct stat	st;
	// char		*tmp;

	if (tools->cmd->args[1] == NULL || !ft_strcmp("~", tools->cmd->args[1]))
	{
		if (chdir(getenv("HOME")) == -1)
		{
			cd_error(tools->cmd->args[1]);
			return (1);
		}
		else
		{
			ft_pwd(tools->env, tools->aloc, tools->cmd->args[1], tools);
		}
	}
	else if (stat(tools->cmd->args[1], &st) != 0)
	{
		cd_error(tools->cmd->args[1]);
		return (1);
	}
	else
	{
		if (chdir(tools->cmd->args[1]) == -1)
		{
			cd_error(tools->cmd->args[1]);
			return (1);
		}
		else
		{
			ft_pwd(tools->env, tools->aloc, tools->cmd->args[1], tools);
		}
	}
	return (0);
}

int	ft_export(t_tools *tools)
{
	int		i;
	int		check;
	int		status;
	char	*value;
	char	*key;
	t_env	*ptr; 

	ptr = *(tools->env);
	i = 0;
	status = 0;
	while (tools->cmd->args[++i])
	{
		key = ft_split(tools->cmd->args[i], '=', tools->aloc)[0];
		value = ft_strchr(tools->cmd->args[i], '=');
		if (value)
			value++;
		check = var_action(key, value, *(tools->env));
		if (check == 1)
			push_to_env(tools, key, value, 1);
		if (check == 2)
			update_var(tools, value, key);
		if (check == 4)
			append_value(tools, key, value);
		if (check == 3)
		{
			status = 1;
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(key, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
	}
	if (i == 1)
		export_display(tools->env, tools->aloc);
	return (status);
}

int	ft_env(t_malloc **aloc, t_env **env, t_cmd *cmd)
{
	t_env	*ptr;

	(void)aloc;
	ptr = *env;
	if (cmd->args[1])
	{
		ft_putstr_fd("env:", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("too many arguments\n", 2);
		return (1);
	}
	while (ptr)
	{
		if (ptr->value && (ptr->type == 0 || ptr->type == 1))
			printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return (0);
}

int	ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	int		i;
	int		status;
	t_env	*ptr;

	i = 0;
	status = 0;
	ptr = *env;
	while (cmd->args[++i])
	{
		if (is_var_exist(cmd->args[i], *env))
			remove_variable(cmd->args[i], env, aloc);
		if (!is_key_valid(cmd->args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
	}
	return (status);
}

void	ft_exit(t_malloc **aloc, t_cmd *cmd, int *status)
{
	char		*s;
	long long	i;

	if (cmd->args[1])
	{
		if (cmd->args[2])
		{
			printf("exit\n");
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			*status = 1;
		}
		else
		{
			s = ft_isnum(cmd->args[1], aloc);
			if (s)
			{
				i = ft_atoi(s);
				printf("exit\n");
				if (errno == ERANGE)
				{
					ft_putstr_fd("minishell: exit: ", 2);
					ft_putstr_fd(s, 2);
					ft_putstr_fd(": numeric argument required\n", 2);
					if (aloc)
					{
						clean_up(aloc, P_ENVIRONMENT);
						clean_up(aloc, P_GARBAGE);
					}
					errno = 0;
					exit(255);
				}
				if (aloc)
				{
					clean_up(aloc, P_ENVIRONMENT);
					clean_up(aloc, P_GARBAGE);
				}
				exit(i % 256);
			}
			else
			{
				printf("exit\n");
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(cmd->args[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				if (aloc)
				{
					clean_up(aloc, P_ENVIRONMENT);
					clean_up(aloc, P_GARBAGE);
				}
				exit(255);
			}
		}
	}
	else
	{
		if (aloc)
		{
			clean_up(aloc, P_ENVIRONMENT);
			clean_up(aloc, P_GARBAGE);
		}
		printf("exit\n");
		exit(*status);
	}
}
