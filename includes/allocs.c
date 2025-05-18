/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:33:05 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/18 12:26:55 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*mmallocc(size_t size, t_malloc **head, int p_type)
{
	void		*pointer;
	t_malloc	*new;


	pointer = malloc(size);
	if (!pointer)
		return (perror("malloc"), exit(1), NULL);
	new = malloc(sizeof(t_malloc));
	if (!new)
	{
		return (perror("malloc"), exit(1), free(pointer), NULL);
	}
	new->next = (*head);
	new->ptr = pointer;
	new->p_type = p_type;
	(*head) = new;
	return (pointer);
}

void	clean_up(t_malloc **head, int t_type)
{
	t_malloc	*nxt;

	(void)t_type;
	if (!(head) || !(*head))
		return ;
	while ((*head))
	{
		free((*head)->ptr);
		nxt = (*head)->next;
		free((*head));
		(*head) = nxt;
	}
	(*head) = NULL;
}

void	free_ptr(t_malloc **head, void *addr)
{
	t_malloc	*ptr;
	t_malloc	*previous;

	ptr = *head;
	previous = NULL;
	while (ptr)
	{
		if (ptr->ptr == addr)
		{
			if (previous)
				previous->next = ptr->next;
			else
				*head = ptr->next;
			free(ptr->ptr);
			free(ptr);
			return ;
		}
		previous = ptr;
		ptr = ptr->next;
	}
}
