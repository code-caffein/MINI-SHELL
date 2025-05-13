
#include "he.h"

int fill1_fourth(t_sp_var *va)
{
	if (va->var->i > 0 && va->var->buffer[0] != '<' && va->var->buffer[0] != '>' && va->var->buffer[0] != '|')
	{
		va->var->buffer[va->var->i] = '\0';
		// printf("{{%s}}\n",v->buffer);
		if (!add_token_with_type(va))
		{
			va->var->i = 0;
			return (0);
		}
		va->var->i = 0;
		va->var->buffer[va->var->i] = '\0';
		if (!ft_isspace(va->var->c))
		{
			// printf("SSSs\n");
			va->var->buffer[va->var->i++] = va->var->c;
			va->var->buffer[va->var->i] = '\0';
		}
		return (1);
	}
	else if (va->var->i > 0 && va->var->buffer[0] == '<' && va->var->c == '<')
	{
		va->var->buffer[va->var->i++] = va->var->c;
		// v->buffer[v->i] = '\0';
		if (!va->line[va->var->j + 1])
		{
			// printf(":::::::::");
			va->var->buffer[va->var->i] = '\0';
			if (!add_token_with_type(va))
			{
				va->var->i = 0;
				return (0);
			}
			va->var->i = 0;
			va->var->buffer[0]='\0';
			return (1);
		}
	}
	return (2);
}
int fill2_fourth(t_sp_var *va)
{
	if (va->var->i > 0 && va->var->buffer[0] == '>' && va->var->c == '>')
	{
		va->var->buffer[va->var->i++] = va->var->c;
		if (!va->line[va->var->j + 1])
		{
			va->var->buffer[va->var->i] = '\0';
			if (!add_token_with_type(va))
			{
				va->var->i = 0;
				return (0);
			}
			va->var->i = 0;
			va->var->buffer[0]='\0';
			return (1);
		}
	}
	else if (va->var->i > 0 && va->var->buffer[0] == '|' && va->var->c == '|')
	{
		va->var->buffer[va->var->i++] = va->var->c;
		if (!va->line[va->var->j + 1])
		{
			va->var->buffer[va->var->i] = '\0';
			if (!add_token_with_type(va))
			{
				va->var->i = 0;
				return (0);
			}
			va->var->i = 0;
			va->var->buffer[0]='\0';
			return (1);
		}
	}
	return (2);
}
int	fill_fill3_fourth(t_sp_var *va)
{
	va->var->buffer[va->var->i] = '\0';
	if (!add_token_with_type(va))
	{
		va->var->i = 0;
		return (0);
	}
	va->var->i = 0;
	if (!ft_isspace(va->var->c))
		{
			va->var->buffer[va->var->i++] = va->var->c;
			va->var->buffer[va->var->i] = '\0';
		}
	return (1);
}

int fill3_fourth(t_sp_var *va)
{
	if (va->var->i > 0 && va->var->buffer[0] == '<' && va->var->c != '<')
	{
		va->var->buffer[va->var->i] = '\0';
		if (!add_token_with_type(va))
		{
			va->var->i = 0;
			return (0);
		}
		va->var->i = 0;
		if (!ft_isspace(va->var->c))
		{
			va->var->buffer[va->var->i++] = va->var->c;
			va->var->buffer[va->var->i] = '\0';
		}
		return (1);
	}
	else if (va->var->i > 0 && va->var->buffer[0] == '>' && va->var->c != '>')
	{
		va->var->buffer[va->var->i] = '\0';
		if (!add_token_with_type(va))
		{
			va->var->i = 0;
			return (0);
		}
		va->var->i = 0;
		if (!ft_isspace(va->var->c))
		{
			va->var->buffer[va->var->i++] = va->var->c;
			va->var->buffer[va->var->i] = '\0';
		}
		return (1);
	}
	else if (va->var->i > 0 && va->var->buffer[0] == '|' && va->var->c != '|')
		return (fill_fill3_fourth(va));
	else
		return (2);
}


int Fourth_condition(t_sp_var *va)
{
    int a;
    int b;
	int c;
    va->var->wait_more_args = false;
    a = fill1_fourth(va);
    if (a == 2)
		b = fill2_fourth(va);
	if (a == 2 && b == 2)
		c = fill3_fourth(va);
    // printf("----------- [%c]-------------\n",v->c);
    if (a != 2)
    {
        if (a == 0)
            return (0);
    }
    else if (b != 2)
    {
        if (b == 0)
            return (0);
    }
	else if (c != 2)
	{
		if (c == 0)
			return 0;
	}
    else if (va->var->i == 0 && !ft_isspace(va->var->c))
        va->var->buffer[va->var->i++] = va->var->c;
        
    return (1);
}
