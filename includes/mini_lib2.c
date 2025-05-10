/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_lib2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:55:09 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/10 15:17:26 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char *ft_isnum(char *s, t_malloc **a)
{
	int	i;
	char **arr;

	arr = ft_split(s, ' ', a);
	if (arr[1])
		return(0);
	i = 0;
	if(s[i] == '-' || s[i] == '+')
		i++;
	while(s && s[i])
	{
		if(!ft_isdigit(s[i]))
			return(0);
		i++;
	}
	return(arr[0]);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

int	ft_isalnum(unsigned char c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		if(write(fd, s, ft_strlen(s)) == -1)
			perror("write:");
}

void swap_string(char **s1, char **s2)
{
	char *s;

	s = (*s2);
	*s2 = *s1;	
	*s1 = s;
}