/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_lib0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:55:09 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/10 14:40:13 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = ft_strlen((char *)src);
	if (!dstsize)
		return (src_len);
	while ((i < dstsize - 1) && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (src_len);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	if (!dst && !dstsize)
		return (ft_strlen((char *)src));
	i = ft_strlen(dst);
	if (dstsize < i + 1)
		return (ft_strlen((char *)src) + dstsize);
	j = 0;
	while ((dstsize > i + 1) && src[j])
		dst[i++] = src[j++];
	dst[i] = 0;
	return (i + ft_strlen((char *)src) - j);
}

char	*ft_strdup(const char *s1, t_malloc **alloc, int p_type)
{
	char	*c;
	size_t	i;
	size_t	l;
	
	if(!s1)
		return(NULL);
	l = ft_strlen((char *) s1);
	i = 0;
	c = (char *) mmallocc(l + 1, alloc, p_type);
	if (!c)
		return (NULL);
	ft_strlcpy(c, s1, l + 1);
	return (c);
}

char	*ft_substr(char const *s, unsigned int start, size_t len, t_malloc **a)
{
	size_t	s_len;
	char	*c;
	size_t	size;

	if (!s)
		return (NULL);
	s_len = ft_strlen((char *) s);
	if (start > s_len)
		return (ft_strdup("", a, P_GARBAGE));
	s_len = ft_strlen((char *) &s[start]);
	size = len + 1;
	if (s_len < len)
		size = s_len + 1;
	c = (char *) mmallocc(size, a, P_GARBAGE);
	if (!c)
		return (NULL);
	ft_strlcpy(c, &s[start], size);
	return (c);
}

char	*ft_strjoin(char const *s1, char const *s2, t_malloc **a)
{
	size_t	siz;
	char	*c = NULL;

	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	else
	{
		siz = ft_strlen((char *) s1) + ft_strlen((char *) s2) + 1;
		c = (char *) mmallocc(siz, a, P_GARBAGE);
		if (!c)
			return (NULL);
		ft_strlcpy(c, s1, ft_strlen((char *) s1) + 1);
		ft_strlcat(c, s2, siz);
	}
	return (c);
}
