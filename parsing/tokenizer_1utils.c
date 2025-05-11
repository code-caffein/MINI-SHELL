#include "he.h"

char	*ft_sttrdup(const char *s1)
{
	char	*c;
	size_t	i;
	size_t	l;
	
	if(!s1)
		return(NULL);
	l = ft_strlen((char *) s1);
	i = 0;
	c = (char *)malloc(l + 1);
	if (!c)
		return (NULL);
	ft_strlcpy(c, s1, l + 1);
	return (c);
}

char	*ft_sttrjoin(char const *s1, char const *s2)
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
		c = (char *) malloc(siz);
		if (!c)
			return (NULL);
		ft_strlcpy(c, s1, ft_strlen((char *) s1) + 1);
		ft_strlcat(c, s2, siz);
	}
	return (c);
}