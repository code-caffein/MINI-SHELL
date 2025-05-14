#include "he.h"

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

void add_expanded_token(t_v *v, t_token **tokens, char *expanded, t_sp_var *va)
{
	t_token *new_token;
	char *tmp = ft_strdup(expanded, &va->allocs, P_GARBAGE);
	new_token = create_new_token(&new_token, tmp, va);
	if (new_token) {
        new_token->type = text; // Set the token type to text
    }
	v->new_token = new_token;
	if (*tokens == NULL)
        *tokens = v->new_token;
    else
   	{
        v->current = *tokens;
        while (v->current->next)
        	v->current = v->current->next;
        v->current->next = v->new_token;
   	}
}

void add_token(t_v *v, t_token **tokens)
{
		v->new_token->value = v->buff;
		if (*tokens == NULL)
        	*tokens = v->new_token;
    	else
   		{
        	v->current = *tokens;
        	while (v->current->next)
        	    v->current = v->current->next;
        	v->current->next = v->new_token;
   		}
}