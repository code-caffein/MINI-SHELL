/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:39:29 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/05 16:58:07 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *push_to_env(t_env **head, char *key, char *value, int type)
{
    t_env *pointer;
    t_env *new;

    new = malloc(sizeof(t_env));
    if (!new)
        return (ft_putstr_fd("var allocation error!", 2), exit(1), NULL);
    new->next = NULL;
    new->key = ft_strdup(key);
    new->type = type;
    new->value = ft_strdup(value); 
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


void push_envp(t_env **head ,char **envp)
{
	char *key;
	char *value;
	int i = -1;

	while (envp[++i])
	{
		key = ft_split(envp[i], '=')[0];
		value = ft_strchr(envp[i], '=') + 1;
		// printf("key = |%s| value = |%s|\n", key, value);
		push_to_env(head, key, value, 0);
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
