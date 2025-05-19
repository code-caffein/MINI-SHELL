/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:39:29 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/19 10:49:41 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*push_to_env(t_tools *tools, char *key, char *value)
{
	t_env	*pointer;
	t_env	*new;

	pointer = NULL;
	new = mmallocc(sizeof(t_env), tools->aloc, P_ENVIRONMENT);
	if (!new)
		return (perror("malloc"), exit(1), NULL);
	new->next = NULL;
	new->key = ft_strdup(key, tools->aloc, P_ENVIRONMENT);
	new->value = ft_strdup(value, tools->aloc, P_ENVIRONMENT);
	if (!(*(tools->env)))
		(*(tools->env)) = new;
	else
	{
		pointer = (*(tools->env));
		while (pointer->next)
			pointer = pointer->next;
		pointer->next = new;
	}
	return (pointer);
}

void	push_envp(t_tools *tools)
{
	char	*key;
	char	*value;
	int		i;

	i = -1;
	while (tools->envp[++i])
	{
		key = ft_split(tools->envp[i], '=', tools->aloc)[0];
		value = ft_strchr(tools->envp[i], '=') + 1;
		push_to_env(tools, key, value);
	}
}

int	count_env_nodes(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	*get_key_value(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(key, env->key))
		{
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}
