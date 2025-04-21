/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_lib1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:57:56 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/21 15:14:08 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			j;
	unsigned char	*us1;
	unsigned char	*us2;

	us1 = (unsigned char *)s1;
	us2 = (unsigned char *)s2;
	j = 0;
	while ((us1[j] && us2[j]) && us1[j] == us2[j])
		j++;
	if (us1[j] < us2[j])
		return (-1);
	if (us1[j] > us2[j])
		return (1);
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			j;
	unsigned char	*us1;
	unsigned char	*us2;

	us1 = (unsigned char *)s1;
	us2 = (unsigned char *)s2;
	j = 0;
	while ((us1[j] && us2[j]) && us1[j] == us2[j] && j < n - 1 && n)
		j++;
	if (us1[j] < us2[j] && n)
		return (-1);
	if (us1[j] > us2[j] && n)
		return (1);
	return (0);
}

int	count_list_size(void *head, size_t offset)
{
	int count = 0;
	while (head)
	{
		head = *(void **)((char *)head + offset);
		count++;
	}
	return count;
}

