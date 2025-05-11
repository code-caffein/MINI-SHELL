#include "he.h"

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static size_t	ft_wordlen(char const *s, char c, size_t *i)
{
	size_t	len;

	len = 0;
	while (s[*i] != c && s[*i])
	{
		len++;
		(*i)++;
	}
	return (len);
}

static void	ft_free(char **strs, size_t j)
{
	while (j > 0)
	{
		j--;
		free(strs[j]);
	}
	free(strs);
}

static char	**ft_fill(char const *s, char c, char **strs, size_t words)
{
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	j = 0;
	while (j < words)
	{
		while (s[i] == c)
			i++;
		len = ft_wordlen(s, c, &i);
		strs[j] = (char *)malloc(sizeof(char) * (len + 1));
		if (!strs[j])
		{
			ft_free(strs, j);
			return (NULL);
		}
		ft_strlcpy(strs[j], &s[i - len], len + 1);
		j++;
	}
	strs[j] = NULL;
	return (strs);
}

char	**ft_spplit(char const *s, char c)
{
	char	**strs;
	size_t	words;

	if (!s)
		return (NULL);
	words = ft_wordcount(s, c);
	strs = (char **)malloc(sizeof(char *) * (words + 1));
	if (!strs)
		return (NULL);
	return (ft_fill(s, c, strs, words));
}