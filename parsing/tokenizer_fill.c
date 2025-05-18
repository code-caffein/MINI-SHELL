
#include "parsing.h"

void first_condtion(t_sp_var *va)
{
	if (va->var->i > 0)
	{
		if (!is_token_separator(va->var->buffer[va->var->i - 1]))
			va->var->wait_more_args = true;
		va->var->buffer[va->var->i] = '\0';
		if (!add_token_with_type(va))// add(&v,env)
 		{
			// free_token_list(&va->var->tokens);
			return ;
		}
		va->var->buffer[0]='\0';
		va->var->i = 0;
	}
	if (va->var->c == '\'')
		va->var->state = SINGLE_QUOTED;
	else if (va->var->c == '"')
		va->var->state = DOUBLE_QUOTED;
	va->var->buffer[va->var->i++] = va->var->c;
	va->var->wait_more_args = true;
}

int	second_condition (t_sp_var *va)
{
	va->var->buffer[va->var->i++] = va->var->c;
	va->var->buffer[va->var->i] = '\0';
	if (!add_token_with_type(va))
	{
		va->var->i = 0;
		va->var->state = UNQUOTED;
		return 0;
	}
	va->var->buffer[0]='\0';
	va->var->state = UNQUOTED;
	va->var->i = 0;
	return 1;
}

int third_condition(t_sp_var *va)
{
	va->var->buffer[va->var->i++] = va->var->c;
	va->var->buffer[va->var->i] = '\0';
	va->var->wait_more_args = true;
	if (!add_token_with_type(va))
	{
		va->var->i = 0;
		va->var->state = UNQUOTED;
		return 0;
	}
	va->var->wait_more_args = false;
	va->var->state = UNQUOTED;
	va->var->buffer[0]='\0';
	va->var->i = 0;
	return 1;
}

int fifth_condition(t_sp_var *va)
{
	if (va->var->i > 0)
	{
		va->var->wait_more_args = false;
		va->var->buffer[va->var->i] = '\0';
		if (!add_token_with_type(va))
			return (0);
		va->var->buffer[0]='\0';
		va->var->i = 0;
	}
	return (1);
}

int sixth_condition(t_sp_var *va)
{
	if (va->var->i > 0)
	{
		va->var->wait_more_args = false;
		va->var->buffer[va->var->i] = '\0';
		if (!add_token_with_type(va))
			return (0);
		va->var->buffer[0]='\0';
		va->var->i = 0;
	}
	return (1);
}