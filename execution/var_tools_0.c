/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:28:51 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/28 16:33:07 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int is_append_export(char *key)
{
	char *s = ft_strchr(key, '+');
	if(s)
	{
		if(s[1] = '\0')
			return(4);//appent opr requieed
		else
			return(3);// var identifier error
	}
}

int var_action(char *key ,char *value, t_env *env)
{
	if(!is_key_valid(key))
		return(3);//DISPAY SYNTAX ERROR
	if(is_key_valid(key) == 2)
		return(4);//APPEND THE NEW VALUE
	if(is_var_exist(key , env) && value)
		return(2);// update its value
	else if (!is_var_exist(key , env))
		return(1);// declare it into export with value 
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

int is_key_valid(char *key)
{
	int i = 0;
	if(!ft_isalpha(key[0]) && key[0] != '_')
		return(0);
	while(key[++i])
	{
		if(!ft_isalnum(key[i]) && key[i] != '_')
		{
			if(key[i] == '+' &&  key[i+1] == '\0')
				return(2);
			else
				return(0);
		}
	}
	return(1);
}