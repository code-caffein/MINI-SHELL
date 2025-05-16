/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/13 12:55:00 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_cd(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	struct stat	st;
	char		*tmp;

	if (cmd->args[1] == NULL || !ft_strcmp("~", cmd->args[1]))
	{
		if (chdir(getenv("HOME")) == -1)
		{
			cd_error(cmd->args[1]);
			return (1);
		}
		else
		{
			ft_pwd(env, aloc, cmd->args[1]);
		}
	}
	else if (stat(cmd->args[1], &st) != 0)
	{
		cd_error(cmd->args[1]);
		return (1);
	}
	else
	{
		if (chdir(cmd->args[1]) == -1)
		{
			cd_error(cmd->args[1]);
			return (1);
		}
		else
		{
			ft_pwd(env, aloc, cmd->args[1]);
		}
	}
	return (0);
}

int	ft_export(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	int		i;
	int		check;
	int		status;
	char	*value;
	char	*key;
	t_env	*ptr; 

	ptr = *env;
	i = 0;
	status = 0;
	while (cmd->args[++i])
	{
		key = ft_split(cmd->args[i], '=', aloc)[0];
		value = ft_strchr(cmd->args[i], '=');
		if (value)
			value++;
		check = var_action(key, value, *env);
		if (check == 1)
			push_to_env(env, aloc, key, value, 1);
		if (check == 2)
			update_var(env, value, key, aloc);
		if (check == 4)
			append_value(env, key, value, aloc);
		if (check == 3)
		{
			status = 1;
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(key, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
	}
	if (i == 1)
		export_display(env, aloc);
	return (status);
}

int	ft_env(t_malloc **aloc, t_env **env, t_cmd *cmd)
{
	t_env	*ptr;

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
