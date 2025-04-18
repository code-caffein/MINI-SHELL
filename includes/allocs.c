/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:33:05 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/17 15:33:22 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *mmallocc(size_t size, t_malloc **head)
{
	void *pointer;
	t_malloc *new;

	pointer = malloc(size);
	if(!pointer)
		return(NULL);
	new = malloc(sizeof(t_malloc));
	if(!new)
		return(free(pointer) ,NULL);
	new->next = (*head);
	new->ptr = pointer;
	(*head) = new;
	return(pointer);
}

void clean_up(t_malloc **head)
{
	t_malloc *nxt;

	while((*head))
	{
		free((*head) -> ptr);
		nxt = (*head) -> next;
		free((*head));
		(*head) = nxt;
	}
	(*head) = NULL;
}