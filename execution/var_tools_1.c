/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:55:49 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/30 16:42:50 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


char *get_key_value(char *key, t_env *env)
{
	while(env)
	{
		if(!ft_strcmp(key, env->key))
		{
			// printf("%s returned\n", env->value);
			return(env->value);
		}
		env = env->next;
	}
	return(NULL);
}

char	**sort_2d_array(char **arr)
{
	int x = -1;
	int y;

	while(arr[++x])
	{
		y = x + 1;
		while(arr[y])
		{
			if(ft_strcmp(arr[x], arr[y]) > 0)
				 swap_string(&arr[x], &arr[y]);
			y++;
		}
	}
	return(arr);
}

char	**keys_to_2darray(t_env * env, int  type)
{
	int siz = count_env_nodes(env, type);
	char **arr = malloc((siz + 1) * sizeof(char *));
	int  i = 0;
	while (env)
	{
		if(env->type == type)
			arr[i++] = ft_strdup(env->key);
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

	
void append_value(t_env **env, char *key, char *value)
{
	key[ft_strlen(key) -1] = 0;
	if(!is_var_exist(key, *env))
	{
		push_to_env(env, key, value, 1);
	}
	else
	{
		update_var(*env, ft_strjoin(get_key_value(key, *env), value), key);
	}
}