/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:28:51 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/26 20:13:21 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int var_action(char *key ,char *value, t_env *env)
{
	
	if(is_key_valid(key))
		return(3);
	if(!value)
	{
		if(is_var_exist(key , env))
			return(0);// do nothing since var exist
		else
			return(1);// declare it into export var
	}
	else
	{
		if(is_var_exist(key , env))
			return(2);// update its value
		else
			return(1);// declare it into export with value 
	}
}

t_env	*get_bef_node(t_env *lst, t_env *node)
{
	t_env	*ptr;

	if (!lst || !(lst -> next))
		return (NULL);
	ptr = lst;
	while (ptr -> next != node)
	{
		ptr = ptr -> next;
	}
	return (ptr);
}

int is_var_exist(char *key, t_env *env)
{
	while (env)
	{
		if(!ft_strcmp(key, env->key))
			return(1);
		env = env->next;
	}
	return(0);
}

void update_var(t_env *env, char *new_value ,char *key)
{
	while(env)
	{
		if(!ft_strcmp(env->key, key))
		{
			env->value = new_value;
			break;
		}
		env = env->next;
	}
}

void remove_variable(char *key, t_env **vars)
{
	t_env *p;
	t_env *helper;

	p = (*vars);
	while (p)
	{
		if(!ft_strcmp(key, p->key))
		{
			if(p == (*vars))
			{
				*vars = p->next;
			}
			else
			{
				helper = get_bef_node(*vars, p);
				helper->next = p->next;
			}
		}
		p = p->next;
	}
}

void *push_to_env(t_env **head, char *key, char *value)
{
	t_env *pointer;
	t_env *new;

	new = malloc(sizeof(t_env));
	if(!new)
		return(printf("var allocation error !"), NULL);
	new->next = NULL;
	new->key = key;
	new->value = value;
	if(!(*head))
		(*head) = new;
	else
	{
		pointer = (*head);
		while(pointer ->next)			
			pointer = pointer->next;
		pointer->next = new;
	}
	return(pointer);
}

int is_key_valid(char *key)
{
	int i = 0;
	if(!ft_isalpha(key[0]) && key[0] != '_')
		return(0);
	while(key[++i])
	{
		if(!ft_isalnum(key[i]) && key[0] != '_')
			return(0);
	}
	return(1);
}