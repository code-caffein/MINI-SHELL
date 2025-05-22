/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:28:51 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/22 10:37:07 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


int	var_action(char *key ,char *value, t_env *env)
{
	if (!is_key_valid(key))
		return (3);
	if (is_key_valid(key) == 2)
		return (4);
	if (is_var_exist(key, env) && value)
		return (2);
	else if (!is_var_exist(key, env))
		return (1);
	return (0);
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

int	is_var_exist(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(key, env->key))
			return (1);
		env = env->next;
	}
	return (0);
}

void	update_var(t_tools *tools, char *new_value ,char *key)
{
	char	*tmp;
	t_env	*ptr;

	ptr = *(tools->env);
	tmp = NULL;
	while (ptr)
	{
		if (!ft_strcmp(ptr->key, key))
		{
			tmp = ptr->value;
			ptr->value = new_value;
			free_ptr(tools->aloc, tmp);
			return ;
		}
		ptr = ptr->next;
	}
	if (!ft_strcmp(key, "PWD") || !ft_strcmp(key, "p.a.t.h")
		|| !ft_strcmp(key, "OLDPWD") || !ft_strcmp(key, "SHLVL"))
	{
		push_to_env(tools, key, new_value);
		free_ptr(tools->aloc, new_value);
	}
}

void	remove_node(t_env *prev, t_env *target, t_env **vars,
			t_malloc **alloc)
{
	if (target == *vars)
		*vars = target->next;
	else if (prev)
		prev->next = target->next;
	free_ptr(alloc, target->key);
	free_ptr(alloc, target->value);
	free_ptr(alloc, target);
}

void	remove_variable(char *key, t_env **vars, t_malloc **alloc)
{
	t_env	*curr;
	t_env	*prev;

	curr = *vars;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(key, curr->key))
		{
			remove_node(prev, curr, vars, alloc);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	is_key_valid(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			if (key[i] == '+' && key[i + 1] == '\0')
				return (2);
			else
				return (0);
		}
	}
	return (1);
}
