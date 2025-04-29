#include "he.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}


char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	char	*bib;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = 0;
	j = 0;
	bib = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!bib)
		return (NULL);
	while (s1[i])
	{
		bib[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		bib[i] = s2[j];
		i++;
		j++;
	}
	bib[i] = '\0';
	return (bib);
}
char	*ft_strdup(const char *s1)
{
	size_t	i;
	size_t	l;
	char	*p;

	l = 0;
	l = ft_strlen(s1);
	p = (char *)malloc(l + 1);
	if (p == NULL)
		return (NULL);
	i = 0;
	while (i < l)
	{
		p[i] = s1[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	char			a;

	a = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == a)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == a)
		return ((char *)&s[i]);
	return (NULL);
}


int		ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}