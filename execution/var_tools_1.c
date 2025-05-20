/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:55:49 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/20 09:51:19 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**sort_2d_array(char **arr)
{
	int	x;
	int	y;

	x = -1;
	while (arr[++x])
	{
		y = x + 1;
		while (arr[y])
		{
			if (ft_strcmp(arr[x], arr[y]) > 0)
				swap_string(&arr[x], &arr[y]);
			y++;
		}
	}
	return (arr);
}

char	**keys_to_2darray(t_env * env, t_malloc **alloc)
{
	int		siz;
	char	**arr;
	int		i;

	siz = count_env_nodes(env);
	arr = mmallocc((siz + 1) * sizeof(char *), alloc, P_GARBAGE);
	i = 0;
	while (env)
	{
		arr[i++] = ft_strdup(env->key, alloc, P_GARBAGE);
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	append_value(t_tools *tools, char *key, char *value)
{
	char	*old_val;
	char	*tmp;

	tmp = NULL;
	key[ft_strlen(key) -1] = 0;
	if (!is_var_exist(key, *(tools->env)))
	{
		push_to_env(tools, key, value);
	}
	else
	{
		old_val = get_key_value(key, *(tools->env));
		tmp = ft_strjoin(old_val, value, tools->aloc);
		update_var(tools, ft_strdup(tmp, tools->aloc, P_ENV), key);
		free_ptr(tools->aloc, old_val);
		free_ptr(tools->aloc, tmp);
	}
}

char	**vars_to_envp(t_tools *tools)
{
	char	**envp;
	t_env	*cursor;
	int		i;

	i = count_env_nodes(*(tools->env));
	cursor = *(tools->env);
	envp = mmallocc((i + 1) * sizeof(char *), tools->aloc, P_GARBAGE);
	envp[i] = NULL;
	i = 0;
	while (cursor)
	{
		envp[i] = ft_strdup((ft_strjoin(ft_strjoin(cursor->key,"=", tools->aloc), cursor->value, tools->aloc)), tools->aloc, P_GARBAGE);
		cursor = cursor->next;
		i++;
	}
	return (envp);
}
