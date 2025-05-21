#include "parsing.h"


int	resize_buffer(char **buf, int *cap, t_sp_var *va)
{
	char	*t;
	
	*cap *= 2;
	t = f_realloc(*buf, *cap, va);
	if (!t)
		return (-1);
	*buf = t;
	return (0);
}

/*
 * Process single character in heredoc_readline
 */
int	process_character(char c, char **buf, int *len, int *cap, t_sp_var *va)
{
	if (*len + 1 >= *cap)
	{
		if (resize_buffer(buf, cap, va) == -1)
			return (-1);
	}
	
	(*buf)[(*len)++] = c;
	if (c == '\n')
		return (1);  // Signal to break the loop
	return (0);
}

/*
 * Read a single character
 */
int	read_single_char(char *c)
{
	int	r;
	
	r = read(0, c, 1);
	return (r);
}


int	heredoc_read_loop(char **buf, int *len, int *cap, t_sp_var *va)
{
	char	c;
	int	r;
	
	while (1)
	{
		r = read_single_char(&c);
		if (r == 0)
			return (0);
		if (r < 0)
			return (handle_read_error(buf));
		
		if (process_character(c, buf, len, cap, va) == 1)
			break;
	}
	return (*len);
}

int	heredoc_readline(char **out, t_sp_var *va)
{
	int	cap = 128, len = 0;
	char	*buf;
	
	buf = mmallocc(cap, &va->allocs, P_GARBAGE);
	if (!buf)
		return (-1);
	int a = heredoc_read_loop(&buf, &len, &cap, va);
	if (a <= 0)
	{
		*out = NULL;
		if (a == -2)
			return (-2);
		else
			return (0);
	}
	
	buf[len] = '\0';
	*out = buf;
	if (!(*out))
		return -3;
	return (len);
}

/*
 * Read loop for heredoc
 */
