/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:06:15 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/23 11:06:50 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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