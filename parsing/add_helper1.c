#include "parsing.h"

int ft_fill_1(t_sp_var *va)
{
	if (va->var->buffer[0] == '>' || va->var->buffer[0] == '<' || va->var->buffer[0] == '|')
	{
		if (!check_operator_validity(va->var->buffer, &va->var->state))
		{
			add_error_token(&va->var->tokens, va);
			return 0;
		}
	}
	return 1;
}

int ft_fill_2(t_v *v, t_sp_var *va)
{

	if (va->var->state == DOUBLE_QUOTED)
		v->new_buff = remove_character(va, '\"');
	else if (va->var->state == SINGLE_QUOTED)
		v->new_buff = remove_character(va, '\'');
	else if (va->var->state == UNQUOTED)
		v->new_buff = ft_strdup(va->var->buffer, &va->allocs, P_GARBAGE);
	if (!v->new_buff)
		return 0;
	return 1;
}
void init_variable(t_v *v, t_token *tokens)
{
    if (!v)
        return;
    v->new_token = NULL;
    v->current = NULL;
    v->prev_token = last_token(tokens);
    v->buff = NULL;
    v->joined = NULL;
    v->quote = false;
    v->new_buff = NULL;
    v->first = '\0';
    v->last = '\0';
    v->tmp = NULL;
    v->expanded_value = NULL;
    v->bib = NULL;
    v->i = 0;
    v->s = 0;
    v->t = 0;
}

t_token *create_new_token(t_token **new_token, char *new_buff, t_sp_var *va)
{
	*new_token = mmallocc(sizeof(t_token), &va->allocs, P_GARBAGE);
	if (!*new_token)
		return NULL;
	(*new_token)->value = new_buff;
	(*new_token)->next = NULL;
	return *new_token;
}

int need_expandd(char *str, t_quote_state *state)
{
	int i = 0;
	if (*state != SINGLE_QUOTED)
	{
		while (str[i])
		{
			if (str[i] == '$')
				return (1);
			i++;
		}
	}
	return (0);
}

t_token *last_token(t_token *token)
{
	t_token	*curr;

	if (!token)
		return (NULL);
	else
	{
		curr = token;
		while (curr->next)
			curr = curr->next;
		return (curr);
	}
}