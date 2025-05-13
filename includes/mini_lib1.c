/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_lib1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:57:56 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/13 14:28:04 by aelbour          ###   ########.fr       */
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

int	count_list_size(void *head, size_t offset)
{
	int	count;

	count = 0;
	while (head)
	{
		head = *(void **)((char *)head + offset);
		count++;
	}
	return (count);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	j;

	j = ft_strlen((char *) s);
	i = 0;
	while (i <= j)
		if (s[i++] == (char)c)
			return ((char *) &s[--i]);
	return (NULL);
}

int	ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}
