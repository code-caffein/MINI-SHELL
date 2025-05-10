/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:55:49 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/10 15:11:13 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

char	**keys_to_2darray(t_env * env, int  type, t_malloc **alloc)
{
	int siz = count_env_nodes(env, type);
	char **arr = mmallocc((siz + 1) * sizeof(char *), alloc, P_GARBAGE);
	int  i = 0;
	while (env)
	{
		if(env->type == type)
			arr[i++] = ft_strdup(env->key, alloc, P_GARBAGE);
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void append_value(t_env **env, char *key, char *value, t_malloc **alloc)
{
	char *old_val;
	char *tmp;

	tmp = NULL;
	key[ft_strlen(key) -1] = 0;
	if(!is_var_exist(key, *env))
	{
		push_to_env(env, alloc, key, value, 1);
	}
	else
	{
		old_val  = get_key_value(key, *env);
		tmp = ft_strjoin(old_val, value, alloc);
		update_var(*env, ft_strdup(tmp, alloc, P_ENVIRONMENT), key, alloc);
		free_ptr(alloc, old_val);
		free_ptr(alloc, tmp);
	}
}