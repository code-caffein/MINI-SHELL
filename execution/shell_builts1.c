/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:23:42 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/20 10:29:51 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	skip_nflags(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	if (!cmd->args[i])
		return (1);
	while (cmd->args[i])
	{
		j = 0;
		if (cmd->args[i][0] == '-')
			j++;
		else
			return (i);
		while (cmd->args[i][j])
		{
			if (cmd->args[i][j] != 'n')
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

int ft_echo(t_cmd *cmd)
{
	int	nl;
	int	i;

	i = skip_nflags(cmd);
	nl = 1;
	if (i == -1)
		return (0);
	else if (i > 1)
		nl = 0;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i],1);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (nl)
		write(1, "\n", 1);
	return (0);
}


int	ft_pwd(t_env **env, t_malloc **a, char *to_path, t_tools *tools)
{
	char	*pwd;
	char	*oldpwd;

	if (env && a && to_path)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			ft_putstr_fd("cd: error retrieving current directory: ", 2);
			perror("getwcd");
			oldpwd = get_key_value("PWD", *env);
			if(oldpwd && ft_strlen(oldpwd))
			{
				update_var(tools, ft_strdup(oldpwd, a, P_ENV), "OLDPWD");
				update_var(tools, ft_strdup(ft_strjoin(ft_strjoin(oldpwd, "/", a), to_path, a), a, P_ENV), "p.a.t.h");
				update_var(tools, ft_strdup(ft_strjoin(ft_strjoin(oldpwd, "/", a), to_path, a), a,P_ENV), "PWD");
			}
			else if(oldpwd)
			{
				update_var(tools, ft_strdup(oldpwd, a, P_ENV), "OLDPWD");
				update_var(tools, ft_strdup(ft_strjoin(oldpwd, to_path, a), a, P_ENV), "p.a.t.h");
				update_var(tools, ft_strdup(ft_strjoin(oldpwd, to_path, a), a, P_ENV), "PWD");
			}
			return 0;
		}
		oldpwd = get_key_value("PWD", *env);
		if (oldpwd)
		{
			update_var(tools, ft_strdup(oldpwd, a, P_ENV), "OLDPWD");
			update_var(tools, ft_strdup(pwd, a, P_ENV), "p.a.t.h");
			update_var(tools, ft_strdup(pwd, a, P_ENV), "PWD");
		}
		free(pwd);
		return (0);
	}
	else
	{
		pwd = get_key_value("p.a.t.h", *env);
		if (pwd)
		{
			printf("%s\n", pwd);
			return (0);
		}
	}
	ft_putstr_fd(PWD_ERROR, 2);
	return (1);
}

void	export_display(t_env **env, t_malloc **a)
{
	char	**arr;
	char	*value;
	int		i;

	arr = keys_to_2darray(*env, a);
	i = 0;
	arr = sort_2d_array(arr);
	while (arr[i])
	{
		value = get_key_value(arr[i], *env);
		if (value && is_key_valid(arr[i]))
			printf("declare -x %s=\"%s\"\n", arr[i], value);
		else if (is_key_valid(arr[i]))
			printf("declare -x %s\n", arr[i]);
		i++;
	}
}
