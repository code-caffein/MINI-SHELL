#include "minishell.h"

static int ft_digit_count(long int i)
{
    int count = 0;

    if (i == 0)
        return 1;
    if (i < 0)
    {
        count++;
        i = -i;
    }
    while (i > 0)
    {
        count++;
        i /= 10;
    }
    return count;
}

char *ft_itoa(int n)
{
    long int nb = n;
    int len = ft_digit_count(nb);
    char *str = malloc(len + 1);

    if (!str)
        return NULL;

    str[len] = '\0';

    if (nb == 0)
        str[0] = '0';
    else
    {
        if (nb < 0)
        {
            str[0] = '-';
            nb = -nb;
        }
        int i = len - 1;
        while (nb > 0)
        {
            str[i--] = (nb % 10) + '0';
            nb /= 10;
        }
    }
    return str;
}

