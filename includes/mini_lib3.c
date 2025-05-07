#include "minishell.h"

static int	ft_digit_count(long int i)
{
	int	count;

	count = 0;
	if (i < 0)
	{
		i *= -1;
		count++;
	}
	while (i > 0)
	{
		i /= 10;
		count++;
	}
	return (count);
}

char		*ft_itoa(int n)
{
	char		*str;
	int			i;
	long int	nb;

	nb = n;
	i = ft_digit_count(nb);
	if (!(str = malloc(i * sizeof(char) + 1)))
		return (0);
	str[i--] = 0;
	if (nb == 0)
	{
		str = malloc(2);
		str[0] = 48;
	}
	if (nb < 0)
	{
		str[0] = '-';
		nb = nb * -1;
	}
	while (nb > 0)
	{
		str[i--] = nb % 10 + '0';
		nb = nb / 10;
	}
	return (str);
}

