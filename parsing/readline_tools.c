#include "parsing.h"

int g_signal_pid;

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	i = 0;
	if (src == dst)
		return (dst);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (!src && !dst)
		return (NULL);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

void	*f_realloc(void *ptr, size_t new_size, t_sp_var *va)
{
	size_t	current_size;
	char	*new_ptr;

	if (ptr == 0)
		return (mmallocc(new_size, &va->allocs, P_GARBAGE));
	current_size = sizeof(ptr);
	if (new_size <= current_size)
		return (ptr);
	new_ptr = mmallocc(new_size, &va->allocs, P_GARBAGE);
	ft_memcpy(ptr, new_ptr, current_size);
	return (new_ptr);
}

/*
 * Function to flush input buffer when a signal is received
 */
int	flush_input_buffer(void)
{
	int	bytes_available;
	char	dummy[256];
	
	while (1)
	{
		if (ioctl(0, FIONREAD, &bytes_available) == -1 || bytes_available <= 0)
			break;
		if (read(0, dummy, sizeof(dummy)) <= 0)
			break;
	}
	return (0);
}

/*
 * Handle interrupted read (signal case)
 */
int	handle_interrupted_read(char **out)
{
	if (g_signal_pid == -2)
		flush_input_buffer();
	
	write(1, "\n", 1);
	*out = NULL;
	return (-2);
}

/*
 * Handle read errors in readline
 */
int	handle_read_error(char **out)
{
	if (errno == EINTR)
		return (handle_interrupted_read(out));
	return (-1);
}