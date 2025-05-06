
#include "he.h"

int fill1_fourth(t_var *v, char *line, t_env *env)
{
	if (v->i > 0 && v->buffer[0] != '<' && v->buffer[0] != '>' && v->buffer[0] != '|')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env))
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
			if (!add_token_with_type(v, env))
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
int fill2_fourth(t_var *v, char *line, t_env *env)
{
	if (v->i > 0 && v->buffer[0] == '>' && v->c == '>')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1])
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(v, env))
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
			if (!add_token_with_type(v, env))
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
int	fill_fill3_fourth(t_var *v, t_env *env)
{
	v->buffer[v->i] = '\0';
	if (!add_token_with_type(v, env))
	{
		v->i = 0;
		return (0);
	}
	v->i = 0;
	return (1);
}

int fill3_fourth(t_var *v, t_env *env)
{
	if (v->i > 0 && v->buffer[0] == '<' && v->c != '<')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env))
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
		if (!add_token_with_type(v, env))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '|' && v->c != '|')
		return (fill_fill3_fourth(v, env));
	else
		return (2);
}


int Fourth_condition(t_var *v, char *line, t_env *env)
{
	int	a;
	int	b;
	int c;
	v->wait_more_args = false;
	a = fill1_fourth(v, line, env);
	b = fill2_fourth(v, line, env);
	c = fill3_fourth(v, env);
	if (a != 2  )
	{
		if (a == 0)
			return (0);
	}if (b != 2)
	{
		if (b == 0)
			return (0);
	}
	if (c != 2)
	{
		if (c == 0)
			return (0);
	}
	else if (v->i == 0)
		v->buffer[v->i++] = v->c;
	return (1);
}