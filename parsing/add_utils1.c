#include "parsing.h"

bool should_expand_token(t_v *v, t_sp_var *va)
{
    return (!(v->prev_token && ft_strcmp(v->prev_token->value, "<<") == 0) 
           && need_expandd(v->new_buff, &va->var->state));
}

void init_first_last(t_v *v, t_sp_var *va, char **static_buffer)
{
	v->expanded_value = expand_env_vars(v->new_buff, va);
	if (ft_strcmp(v->expanded_value, "") != 0)
	{
		v->first = v->expanded_value[0];
		v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
	}
	v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
	v->i = 0;
	int l = 0;
	while (v->bib[l])
			l++;
	if (!ft_isspace(v->first) && l != 1)
	{
		*static_buffer = ft_strjoin(v->buff, v->bib[v->i++], &va->allocs);
		add_expanded_token(v, &va->var->tokens, *static_buffer, va);
		*static_buffer = NULL;
	}
	else if (!ft_isspace(v->first) && l == 1 && va->var->wait_more_args)
	{
		*static_buffer = ft_strjoin(v->buff, v->bib[v->i++], &va->allocs);
		v->i = -10;
	}
	else if (!ft_isspace(v->first) && l == 1 && !va->var->wait_more_args)
	{
		*static_buffer = ft_strjoin(v->buff, v->bib[v->i++], &va->allocs);
		add_expanded_token(v, &va->var->tokens, *static_buffer, va);
		*static_buffer = NULL;
	}
	else
		add_expanded_token(v, &va->var->tokens, v->buff, va);
	v->s = v->i;
}

void p_ex_with_buffer(t_v *v, t_sp_var *va, char **static_buffer)
{
	init_first_last(v, va, static_buffer);
	if (va->var->wait_more_args)
	{
		if (v->i != -10){

		while (v->bib[v->i])
			v->i++;
		v->t = v->i - 1;
		v->i = v->s;
		while(v->bib[v->i])
		{
			if (v->i != v->t)
				add_expanded_token(v, &va->var->tokens, v->bib[v->i++], va);
			else
				break;
		}
		if (ft_isspace(v->last) && v->t != 0)
			add_expanded_token(v, &va->var->tokens, v->bib[v->t], va);
		else if (v->t != 0)
			*static_buffer = v->bib[v->t];
		}
	}
	else
	{
		while (v->bib[v->i])
			add_expanded_token(v, &va->var->tokens, v->bib[v->i++], va);
		*static_buffer = NULL;
	}
}

void	p_with_buffer(t_v *v, t_sp_var *va, char **static_buffer, bool *QUOTE)
{
	if (should_expand_token(v, va) )
		p_ex_with_buffer(v, va, static_buffer);
	else
	{
		if(va->var->wait_more_args)
			*static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
		else 
		{
			if (!(*QUOTE))
				v->new_token->need_expand = true;
			*static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
			v->buff = *static_buffer;
			add_token(v, &va->var->tokens);
			*static_buffer = NULL;
			*QUOTE = false;
		}
	}
}

void update_quote_state(t_sp_var *va, bool *QUOTE)
{
	if (va->var->state != UNQUOTED && (!(*QUOTE) || (*QUOTE)))
		*QUOTE = true; 
	else if(va->var->state == UNQUOTED && !(*QUOTE))
		*QUOTE = false;
	else if(va->var->state == UNQUOTED && (*QUOTE))
		*QUOTE = true;
}