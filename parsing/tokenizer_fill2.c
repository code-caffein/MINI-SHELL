
#include "he.h"

int fill1_fourth(t_var *v, char *line)
{
	if (v->i > 0 && v->buffer[0] != '<' && v->buffer[0] != '>' && v->buffer[0] != '|')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(&v->tokens, v->buffer, &v->state, v->wait_more_args))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '<' && v->c == '<')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1])
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(&v->tokens, v->buffer, &v->state, v->wait_more_args))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			return (1);
		}
	}
	return (2);
}
int fill2_fourth(t_var *v, char *line)
{
	if (v->i > 0 && v->buffer[0] == '>' && v->c == '>')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1])
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(&v->tokens, v->buffer, &v->state, v->wait_more_args))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			return (1);
		}
	}
	else if (v->i > 0 && v->buffer[0] == '|' && v->c == '|')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1])
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(&v->tokens, v->buffer, &v->state, v->wait_more_args))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			return (1);
		}
	}
	return (2);
}
int	fill_fill3_fourth(t_var *v)
{
	v->buffer[v->i] = '\0';
	if (!add_token_with_type(&v->tokens, v->buffer, &v->state, v->wait_more_args))
	{
		v->i = 0;
		return (0);
	}
	v->i = 0;
	return (1);
}

int fill3_fourth(t_var *v)
{
	if (v->i > 0 && v->buffer[0] == '<' && v->c != '<')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(&v->tokens, v->buffer, &v->state, v->wait_more_args))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '>' && v->c != '>')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(&v->tokens, v->buffer, &v->state, v->wait_more_args))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '|' && v->c != '|')
		return (fill_fill3_fourth(v));
	else
		return (2);
}


int Fourth_condition(t_var *v, char *line)
{
	int	a;
	int	b;
	int c;

	a = fill1_fourth(v, line);
	b = fill2_fourth(v, line);
	c = fill3_fourth(v);
	if (a != 2  )
	{
		if (a == 0)
			return (0);
	}else if (b != 2)
	{
		if (b == 0)
			return (0);
	}
	else if (c != 2)
	{
		if (c == 0)
			return (0);
	}
	else if (v->i == 0)
		v->buffer[v->i++] = v->c;
	return (1);
}