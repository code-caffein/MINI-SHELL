#include "he.h"

int first_if(t_sp_var *va)
{
	if (va->var->state == UNQUOTED && (va->var->c == '\'' || va->var->c == '"'))
			return (1);
	return (0);
}

int second_if(t_sp_var *va)
{
	if ((va->var->state == SINGLE_QUOTED && va->var->c == '\'' && (va->line[va->var->j + 1] != '\'' && va->line[va->var->j  + 1] != '"' && (is_token_separator(va->line[va->var->j + 1]) || !va->line[va->var->j + 1]))) ||
                (va->var->state == DOUBLE_QUOTED && va->var->c == '"'  && (va->line[va->var->j + 1] != '\'' && va->line[va->var->j  + 1] != '"' && (is_token_separator(va->line[va->var->j + 1]) || !va->line[va->var->j + 1]))))
		return (1);
	return (0);
}

int third_if(t_sp_var *va)
{
	if ((va->var->state == SINGLE_QUOTED && va->var->c == '\'' && va->line[va->var->j + 1] && (va->line[va->var->j + 1] == '\'' || va->line[va->var->j + 1] == '"' || !is_token_separator(va->line[va->var->j+1]))) || 
				(va->var->state == DOUBLE_QUOTED && va->var->c == '"' && va->line[va->var->j + 1] && (va->line[va->var->j + 1] == '\'' || va->line[va->var->j + 1] == '"' || !is_token_separator(va->line[va->var->j+1]))))
			return (1);
	return (0);
}

int fourth_if(t_sp_var *va)
{
	if (va->var->state == UNQUOTED && (ft_strchr("|<>", va->var->c) || (va->var->buffer[0] == '<' || va->var->buffer[0] == '>' ||va->var->buffer[0] == '|')))
			return (1);
	return (0);
}

int while_part1(t_sp_var *va)
{
	if (first_if(va))
	{
		first_condtion(va);
		return (1);
	}
	else if (second_if(va))
	{
		va->var->wait_more_args = false;
		if (!second_condition(va))
			return (0);
		return (1);
	}
	else if (third_if(va))
	{
		if (!third_condition(va))
			return (0);
		return (1);
	}
	else if (fourth_if(va))
	{
		if (!Fourth_condition(va))
			return (0);
		return (1);
	}
	return (2);
}