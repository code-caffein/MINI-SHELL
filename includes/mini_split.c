/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:56 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/22 10:49:23 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_seps(size_t *i, char const *s, char c)
{
	while (s[*i] && s[*i] == c)
		(*i)++;
}

static size_t	count_cols(char const *s, char c)
{
	size_t	t;
	size_t	i;

	t = 0;
	i = 0;
	if (!s[i])
		return (0);
	skip_seps(&i, s, c);
	while (s[i])
	{
		if (s[i] == c)
		{
			skip_seps(&i, s, c);
			t++;
		}
		else
			i++;
	}
	if (s[i - 1] != c)
		t++;
	return (t);
}

static int	check_crash(char **arr, size_t i)
{
	if (!arr[i])
	{
		while (i > 0)
		{
			i--;
			free(arr[i]);
		}
		free(arr);
		return (0);
	}
	return (1);
}

static int	ft_store(t_split split)
{
	size_t	i;
	size_t	j;
	size_t	l;

	j = 0;
	i = 0;
	skip_seps(&i, split.s, split.c);
	l = i;
	while (j < split.cols)
	{
		if (split.s[i] == split.c || !split.s[i])
		{
			split.arr[j] = ft_substr(split.s, l, i - l, split.a);
			if (!check_crash(split.arr, j))
				return (0);
			skip_seps(&i, split.s, split.c);
			l = i;
			j++;
		}
		else
			i++;
	}
	return (1);
}

char	**ft_split(char const *s, char c, t_malloc **alloc)
{
	size_t	cols;
	char	**arr;
	t_split	split;

	if (!s)
		return (NULL);
	cols = count_cols(s, c);
	arr = (char **)mmallocc((cols + 1) * sizeof(char *), alloc, P_GARBAGE);
	if (!arr)
		return (NULL);
	arr[cols] = NULL;
	if (cols)
	{
		split = (t_split){arr, s, c, cols, alloc};
		if (ft_store(split))
			return (arr);
		else
			return (NULL);
	}
	return (arr);
}
