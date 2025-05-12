/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:28:51 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/12 12:30:46 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


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
	return(0);// declare it into export with value 
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

void update_var(t_env **env, char *new_value ,char *key, t_malloc **alloc)
{
	char *tmp;

	t_env *ptr;
	
	ptr = *env;
	tmp = NULL;
	while(ptr)
	{
		if(!ft_strcmp(ptr->key, key))
		{
			tmp	 = ptr->value;
			ptr->value = new_value;
			free_ptr(alloc, tmp);
			return;
		}
		ptr = ptr->next;
	}
	if(!ft_strcmp(key, "PWD") || !ft_strcmp(key, "p.a.t.h") || !ft_strcmp(key, "OLDPWD"))
		push_to_env(env, alloc, key, new_value, 0);
	free_ptr(alloc, new_value);
}

void remove_variable(char *key, t_env **vars, t_malloc **alloc)
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
				free_ptr(alloc, p->key);
				free_ptr(alloc, p->value);
				free_ptr(alloc, p);
				break;
			}
			else
			{
				helper = get_bef_node(*vars, p);
				helper->next = p->next;
				free_ptr(alloc, p->key);
				free_ptr(alloc, p->value);
				free_ptr(alloc, p);
				break;
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