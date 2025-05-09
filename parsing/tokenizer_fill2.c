
#include "he.h"

int fill1_fourth(t_var *v, char *line, t_env *env, int status)
{
	if (v->i > 0 && v->buffer[0] != '<' && v->buffer[0] != '>' && v->buffer[0] != '|')
	{
		v->buffer[v->i] = '\0';
		// printf("{{%s}}\n",v->buffer);
		if (!add_token_with_type(v, env,  status))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		v->buffer[0]='\0';
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '<' && v->c == '<')
	{
		v->buffer[v->i++] = v->c;
		// v->buffer[v->i] = '\0';
		if (!line[v->j + 1])
		{
			// printf(":::::::::");
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(v, env,  status))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			v->buffer[0]='\0';
			return (1);
		}
	}
	return (2);
}
int fill2_fourth(t_var *v, char *line, t_env *env, int status)
{
	if (v->i > 0 && v->buffer[0] == '>' && v->c == '>')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1])
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(v, env,  status))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			v->buffer[0]='\0';
			return (1);
		}
	}
	else if (v->i > 0 && v->buffer[0] == '|' && v->c == '|')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1])
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(v, env,  status))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			v->buffer[0]='\0';
			return (1);
		}
	}
	return (2);
}
int	fill_fill3_fourth(t_var *v, t_env *env, int status)
{
	v->buffer[v->i] = '\0';
	if (!add_token_with_type(v, env,  status))
	{
		v->i = 0;
		return (0);
	}
	v->i = 0;
	if (!ft_isspace(v->c))
		{
			v->buffer[v->i++] = v->c;
			v->buffer[v->i] = '\0';
		}
	return (1);
}

int fill3_fourth(t_var *v, t_env *env, int status)
{
	if (v->i > 0 && v->buffer[0] == '<' && v->c != '<')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env,  status))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		if (!ft_isspace(v->c))
		{
			v->buffer[v->i++] = v->c;
			v->buffer[v->i] = '\0';
		}
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '>' && v->c != '>')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env,  status))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		if (!ft_isspace(v->c))
		{
			v->buffer[v->i++] = v->c;
			v->buffer[v->i] = '\0';
		}
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '|' && v->c != '|')
		return (fill_fill3_fourth(v, env,  status));
	else
		return (2);
}


int Fourth_condition(t_var *v, char *line, t_env *env, int  status)
{
    int a;
    int b;
	int c;
    v->wait_more_args = false;
    a = fill1_fourth(v, line, env,  status);
    b = fill2_fourth(v, line, env,  status);
	c = fill3_fourth(v, env,  status);
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
    else if (v->i == 0 && !ft_isspace(v->c))
        v->buffer[v->i++] = v->c;
        
    return (1);
}