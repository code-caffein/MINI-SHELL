/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:39:29 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/10 15:16:38 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *push_to_env(t_env **head, t_malloc **alloc, char *key, char *value, int type)
{
    t_env *pointer = NULL;
    t_env *new;

    new = mmallocc(sizeof(t_env), alloc, P_ENVIRONMENT);
    if (!new)
        return (ft_putstr_fd("var allocation error!", 2), exit(1), NULL);
    new->next = NULL;
    new->key = ft_strdup(key, alloc, P_ENVIRONMENT);
    new->type = type;
    new->value = ft_strdup(value, alloc, P_ENVIRONMENT); 
    if (!(*head))
        (*head) = new;
    else
    {
        pointer = (*head);
        while (pointer->next)
            pointer = pointer->next;
        pointer->next = new;
    }
    return pointer;
}

void push_envp(t_env **head ,char **envp, t_malloc **a)
{
	char *key;
	char *value;
	int i = -1;

	while (envp[++i])
	{
		key = ft_split(envp[i], '=', a)[0];
		value = ft_strchr(envp[i], '=') + 1;
		// printf("key = |%s| value = |%s|\n", key, value);
		push_to_env(head, a, key, value, 0);
	}
}

int count_env_nodes(t_env *env, int type)
{
	int count;

	count  = 0;
	while(env)
	{
		if(env->type ==  type)
			count++;
		env = env->next;
	}
	return(count);
}

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
