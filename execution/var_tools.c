/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:28:51 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/24 19:56:10 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int var_action(char *s, t_env *env)
{
	int i = 0;
	char *key = ft_strchr(s, '=');
	if(!key)
	{
		if(is_var_exist(s, env))
			return(0);// do nothing since var exist
		else
			return(1);// declare it into var envs
	}
	else
	{
		if(!ft_strlen(key))
			return(2);//empty the variable
		else
			return(3);// assign it to NEW VALUE
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

int is_var_exist(char *key, t_env *vars)
{
	while (vars)
	{
		if(!ft_strcmp(key, vars->key))
			return(1);
		vars = vars->next;
	}
	return(0);
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
	void *pointer;
	t_env *new;

	new = malloc(sizeof(t_malloc));
	if(!new)
		return(printf("var allocation error !"), NULL);
	new->next = (*head);
	new->key = key;
	new->value = value;
	(*head) = new;
	return(pointer);
}
