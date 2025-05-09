/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:23:42 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/09 11:02:37 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


int skip_nflags(t_cmd *cmd)
{
	int i;
	int j;

	i = 1;
	if(!cmd->args[i])
		return(1);
	while(cmd->args[i])
	{
		j = 0;
		if(cmd->args[i][0] == '-')
			j++;
		while(cmd->args[i][j])
		{
			if(cmd->args[i][j] != 'n')
				return(i);//if its 1 then there arr no flags otherwise this cmd have n flags
			j++;
		}
		i++;
	}
	return(-1);//in case of they arr all flags -nnn -n -nnnnnnn
}

int ft_echo(t_cmd *cmd)
{
	int nl;
	int i;
	i = skip_nflags(cmd);
	nl = 1;
	if(i == -1)
		return(0);
	else if(i > 1)
		nl = 0;
	while(cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i],1);
		if(cmd->args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if(nl)
		write(1, "\n", 1);
	return(0);
}

int ft_pwd()
{
	char *pwd;
	
	pwd = getcwd(NULL, 0);
	if(pwd)
	{
		printf("%s\n", pwd);
		return(0);
	}
	else
	{
		write(2, "minishell: pwd: ", 16);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		return(1);
	}
}

void export_display(t_env **env)
{
    char **arr = keys_to_2darray(*env, 0);
    int i = 0;
    char *value;

    arr = sort_2d_array(arr);
    while (arr[i])
    {
        value = get_key_value(arr[i], *env);
        if (value)
            printf("declare -x %s=\"%s\"\n", arr[i], value);
        else
            printf("declare -x %s=\n", arr[i]);
        i++;
    }
    arr = keys_to_2darray(*env, 1);
    arr = sort_2d_array(arr);
    i = 0;
    while (arr[i])
    {
        value = get_key_value(arr[i], *env);
        if (value)
            printf("declare -x %s=\"%s\"\n", arr[i], value);
        else
            printf("declare -x %s\n", arr[i]);
        i++;
    }
}

