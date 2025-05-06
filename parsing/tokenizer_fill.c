
#include "he.h"

void first_condtion(t_var *v, t_env *env)
{
	if (v->i > 0)
	{
		if (!is_token_separator(v->buffer[v->i - 1]))
			v->wait_more_args = true;
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env))// add(&v,env)
 		{
			free_token_list(&v->tokens);
			return ;
		}
		v->i = 0;
	}
	if (v->c == '\'')
		v->state = SINGLE_QUOTED;
	else if (v->c == '"')
		v->state = DOUBLE_QUOTED;
	v->buffer[v->i++] = v->c;
	v->wait_more_args = true;
}

int	second_condition (t_var *v, t_env *env)
{
	v->buffer[v->i++] = v->c;
	v->buffer[v->i] = '\0';
	if (!add_token_with_type(v, env))
	{
		v->i = 0;
		v->state = UNQUOTED;
		return 0;
	}
	v->state = UNQUOTED;
	v->i = 0;
	return 1;
}

int third_condition(t_var *v, t_env *env)
{
	v->buffer[v->i++] = v->c;
	v->buffer[v->i] = '\0';
	v->wait_more_args = true;
	if (!add_token_with_type(v, env))
	{
		v->i = 0;
		v->state = UNQUOTED;
		return 0;
	}
	v->wait_more_args = false;
	v->state = UNQUOTED;
	v->i = 0;
	return 1;
}

int fifth_condition(t_var *v, t_env *env)
{
	if (v->i > 0)
	{
		v->wait_more_args = false;
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env))
			return (0);
		v->i = 0;
	}
	return (1);
}

int sixth_condition(t_var *v, t_env *env)
{
	if (v->i > 0)
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env))
			return (0);
		v->i = 0;
	}
	return (1);
}