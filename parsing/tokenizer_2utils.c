#include "he.h"

int ft_fill_1(t_token *tokens, char *buffer, t_quote_state *state)
{
	if (buffer[0] == '>' || buffer[0] == '<' || buffer[0] == '|')
	{
		if (!check_operator_validity(buffer, state))
		{
			add_error_token(tokens);
			return 0;
		}
	}
	return 1;
}

int ft_fill_2(t_v *v, char *buffer, t_quote_state *state)
{

	if (*state == DOUBLE_QUOTED)
		v->new_buff = remove_character(buffer, '\"');
	else if (*state == SINGLE_QUOTED)
		v->new_buff = remove_character(buffer, '\'');
	else if (*state == UNQUOTED)
		v->new_buff = ft_strdup(buffer);
	if (!v->new_buff)
		return 0;
	return 1;
}
void init_variable(t_v *v)
{
	v->new_token = NULL;
	v->current = NULL;
	v->buff = NULL;
	v->joined = NULL;
	v->new_buff = NULL;
}
t_token *create_new_token(t_token **new_token, char *new_buff)
{
	*new_token = malloc(sizeof(t_token));
	if (!*new_token)
		return NULL;
	(*new_token)->value = new_buff;
	(*new_token)->next = NULL;
	return *new_token;
}

int fill1_red_pip_txt(t_v *v, t_quote_state *state)
{
	if (ft_strcmp(v->new_buff, ">") == 0 && *state == UNQUOTED)
    {
        v->new_token->type = red;
        v->new_token->value = v->new_buff;
        v->new_token->heredoc = false;
		return 1;
	}
	else if (ft_strcmp(v->new_buff, "<<") == 0 && *state == UNQUOTED)
	{
		v->new_token->type = red;
		v->new_token->value = v->new_buff;
		v->new_token->heredoc = true;
		return 1;
    }
    else if (ft_strcmp(v->new_buff, "<") == 0 && *state == UNQUOTED)
    {
        v->new_token->type = red;
        v->new_token->value = v->new_buff;
        v->new_token->heredoc = false;
		return 1;
    }
	return 0;
}

void fill2_red_pip_txt(t_v *v, t_quote_state *state)
{
	if (ft_strcmp(v->new_buff, ">>") == 0 && *state == UNQUOTED)
    {
        v->new_token->type = red;
        v->new_token->value = v->new_buff;
        v->new_token->heredoc = false;
	}
	else if (ft_strcmp(v->new_buff, "|") == 0 && *state == UNQUOTED)
		v->new_token->type = pip;
	else
    {
        v->new_token->type = text;
        v->new_token->heredoc = false;
        v->new_token->value = v->new_buff;
    }
}

void red_pip_txt(t_v *v , t_quote_state *state)
{
	if (!fill1_red_pip_txt(v, state))
		fill2_red_pip_txt(v, state);
}

void fill1_add_token(t_v *v)
{
	if (v->buff)
    	{
    		v->joined = ft_strjoin(v->new_token->value, v->buff);
        	free(v->buff);
        	v->buff = v->joined;
    	}
    	else
        	v->buff = ft_strdup(v->new_token->value);
    	if (!v->buff)
    	{
        	free(v->new_token->value);
        	free(v->new_token);
        	return 0;
    	}
    	free(v->new_token->value);
    	free(v->new_token);
}

void add_token(t_v *v, t_token **tokens, bool wait_more_args)
{
	if (wait_more_args)
		fill1_add_token(v);
	else
	{
		if (v->buff)
		{
			v->new_token->value = ft_strjoin(v->buff, v->new_token->value);
			free(v->buff);
			v->buff = NULL;
		}
		else
			v->new_token->value = ft_strdup(v->new_buff);
		if (*tokens == NULL)
		{
        	*tokens = v->new_token;
    	}
    	else
   		{
        	v->current = *tokens;
        	while (v->current->next)
        	    v->current = v->current->next;
        	v->current->next = v->new_token;
   		}
	}
}

int add_token_with_type(t_token **tokens, char *buffer, t_quote_state *state, bool wait_more_args)
{
	t_v *v;

	if (buffer[0] == '\0')
		return 0;
	if (ft_fill_1(*tokens, buffer, state) == 0)
		return 0;//syntax error
	if (!ft_fill_2(v, buffer, state))
		return 0;//malloc failed
	v->new_token = create_new_token(&v->new_token, v->new_buff);
	if (!v->new_token)
	{
		free(v->new_buff);
		return 0;//malloc failed
	}
	red_pip_txt(v, state);
	expand_variables(v->new_token, state);
	add_token(v, tokens, wait_more_args);
	return 1;
}
