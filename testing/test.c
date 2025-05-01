#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
// #include"../parsing/he.h"

typedef struct s_cmd {
    char *name;
    char **args;
} t_cmd;


size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

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

char	*ft_strdup(const char *s1)
{
	char	*c;
	size_t	i;
	size_t	l;

	l = ft_strlen((char *) s1);
	i = 0;
	c = (char *) malloc(l + 1);
	if (!c)
		return (NULL);
	ft_strlcpy(c, s1, l + 1);
	return (c);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*c;
	size_t	size;

	if (!s)
		return (NULL);
	s_len = ft_strlen((char *) s);
	if (start > s_len)
		return (ft_strdup(""));
	s_len = ft_strlen((char *) &s[start]);
	size = len + 1;
	if (s_len < len)
		size = s_len + 1;
	c = (char *) malloc(size);
	if (!c)
		return (NULL);
	ft_strlcpy(c, &s[start], size);
	return (c);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	siz;
	char	*c;

	if (!s1 || !s2)
		return (NULL);
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

static int	ft_store(char **arr, const char *s, char c, size_t cols)
{
	size_t	i;
	size_t	j;
	size_t	l;

	j = 0;
	i = 0;
	skip_seps(&i, s, c);
	l = i;
	while (j < cols)
	{
		if (s[i] == c || !s[i])
		{
			arr[j] = ft_substr(s, l, i - l);
			if (!check_crash(arr, j))
				return (0);
			skip_seps(&i, s, c);
			l = i;
			j++;
		}
		else
			i++;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	size_t	cols;
	char	**arr;

	if (!s)
		return (NULL);
	cols = count_cols(s, c);
	arr = (char **)malloc((cols + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	arr[cols] = NULL;
	if (cols)
	{
			if (ft_store(arr, s, c, cols))
				return (arr);
			else
				return (NULL);
		}
		return (arr);
	}
	


char *get_executable_path(char *str)
{
	int  i;
	char *s = getenv("PATH");
	char **paths = ft_split(s,':');
	char *check;

	i = -1;
	while(paths[++i])
	{
		check = ft_strjoin(paths[i], ft_strjoin("/", str));
		if(!access(check, X_OK))
			return(check);
	}
	return(NULL);
}

int main(void)
{
	t_cmd example;
	int pid;
	char *path;
	char *args[] = {"env",NULL};

	example.args = args;
	example.name = args[0];
	pid = fork();
	if(pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
	}
	else
	{
		int i;
		i = access(example.name, X_OK);
		if(i == -1)
		{
			path = get_executable_path(example.name);
			if(path)
			{
				printf("executable path found = %s\n", path);
				if(execve(path, example.args, NULL) == -1)
					return(printf("execve failed\n") ,-1);
			}
			else
			{
				printf("command not found !\n");
			}
		}
		else
		{
			if(execve(example.name, example.args, NULL) == -1)
				return(printf("execve failed\n") ,-1);
		}

	}
}

// int main(void)
// {
// 	int p[2];
// 	char buff[1000];
// 	int n;

// 	pipe(p);

// 	write(p[1], "hello aymane\n", 13);
// 	n = read(p[0], buff, 1000);
// 	if(n >= 0)
// 	{
// 		buff[n] = 0;
// 		printf("read %i bytes from p[0], text : %s\n", n, buff);
// 	}
// }