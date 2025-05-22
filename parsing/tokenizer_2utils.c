#include "parsing.h"


static void	p_ex_without_buffer_fill(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (!v->bib || !v->bib[0])
	{
		*static_buffer = NULL;
		return;
	}
	v->i = 0;
	while (v->bib[v->i])
		v->i++;
	v->t = v->i - 1;
	v->i = 0;
	while(v->bib[v->i])
	{
		if (v->i != v->t)
			add_expanded_token(v, &va->var->tokens, v->bib[v->i++], va);
		else
			break;
	}
	if (ft_isspace(v->last))
		add_expanded_token(v, &va->var->tokens, v->bib[v->t], va);
	else
		*static_buffer = v->bib[v->t];
}

void p_ex_without_buffer(t_v *v, t_sp_var *va, char **static_buffer)
{
	v->expanded_value = expand_env_vars(v->new_buff, va);
	if (ft_strcmp(v->expanded_value, "") != 0)
		v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
	v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
	if (va->var->wait_more_args)
		p_ex_without_buffer_fill(v, va, static_buffer);
	else
	{
		v->s = -1;
		while (v->bib[++v->s])
			add_expanded_token(v, &va->var->tokens, v->bib[v->s], va);
		*static_buffer = NULL;
	}
}


void	p_without_buffer(t_v *v, t_sp_var *va, char **static_buffer, bool *QUOTE)
{
	if (should_expand_token(v, va))
		p_ex_without_buffer(v, va, static_buffer);
	else
	{
		if (va->var->wait_more_args)
			*static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
		else{
			if (!(*QUOTE))
				v->new_token->need_expand = true;
			v->buff = v->new_buff;
			add_token(v, &va->var->tokens);
			v->buff = NULL;
			*static_buffer = NULL;
		}
		*QUOTE = false;
	}

}

bool ft_ambiguous_red(t_v *v)
{
    if (v->prev_token && v->prev_token->value)
    {
        if (strcmp(v->prev_token->value, ">") == 0 || 
            strcmp(v->prev_token->value, "<") == 0 || 
            strcmp(v->prev_token->value, ">>") == 0)
        {
            return true;
        }
    }
    return false;
}


void add_anbiguous_token(t_v *v, t_token **tokens)
{
		v->new_token->value = v->buff;
		v->new_token->syn_err=false;
		v->new_token->ambiguous = true;
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

int add_token_with_type(t_sp_var *va)
{
	static char	*static_buffer = NULL;
	static bool	QUOTE = false;
	static bool ambiguous_red = false;
	static bool ambiguous = false;
	t_v *v;
    
	v = init_token_vars(va, &QUOTE);

	if (!ambiguous_red)
		ambiguous_red = ft_ambiguous_red(v);
	v->ambiguous = ambiguous;
	if (!v)
		return 0;
	if (static_buffer)
	{
		v->buff = static_buffer;
		static_buffer = NULL;
	}

	if (!prepare_token(va, v))
		return (0);


	if(v->buff)
	{
		if (should_expand_token(v, va) && ambiguous_red && va->var->state == UNQUOTED)
		{
			v->expanded_value = expand_env_vars(v->new_buff, va);
			if (ft_strcmp(v->expanded_value, "") != 0)
			{
				v->first = v->expanded_value[0];
				v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
			}
			v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
			while (v->bib[v->i])
				v->i++;
			if (v->i > 1)
				ambiguous = true;
			if (va->var->wait_more_args && ambiguous)
			{
				static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
				return (1);
			}
			else if (va->var->wait_more_args && v->i == 0)
			{
				static_buffer = ft_strjoin(v->buff, "", &va->allocs);
				return (1);
			}
			else if (va->var->wait_more_args && !ambiguous)
			{

				if (!ft_isspace(v->first) && !ft_isspace(v->last))
				{
					static_buffer = ft_strjoin(v->buff, v->bib[0], &va->allocs);
				}
				else if (ft_isspace(v->first) && ft_isspace(v->last))
				{
					add_expanded_token(v, &va->var->tokens, v->buff, va);
					add_expanded_token(v, &va->var->tokens, v->bib[0], va);
					static_buffer = NULL;
				}else if (!ft_isspace(v->first) && ft_isspace(v->last))
				{
					static_buffer = ft_strjoin(v->buff, v->bib[0], &va->allocs);
					add_expanded_token(v, &va->var->tokens, static_buffer, va);
					static_buffer = NULL;
				}else if (ft_isspace(v->first) && !ft_isspace(v->last))
				{
					add_expanded_token(v, &va->var->tokens, static_buffer, va);
					static_buffer = v->buff;
				}
				return (1);
			}
			else if (!va->var->wait_more_args && v->i == 0)
			{
				add_token(v, &va->var->tokens);
				return (1);
			}
			else if (!va->var->wait_more_args && ambiguous)
			{
				static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
				v->buff = static_buffer;
				if (ambiguous)
					v->ambiguous = true;
				ambiguous = false;
				add_anbiguous_token(v, &va->var->tokens);

				return (1);
			}
			else if (!va->var->wait_more_args && !ambiguous)
			{
				if (ft_isspace(v->first))
				{
					add_expanded_token(v, &va->var->tokens, v->buff, va);
					add_expanded_token(v, &va->var->tokens, v->bib[0], va);
					static_buffer = NULL;
				}
				else
				{
					static_buffer = ft_strjoin(v->buff, v->bib[0], &va->allocs);
					add_expanded_token(v, &va->var->tokens, static_buffer, va);
					static_buffer = NULL;
				}
				return (1);
			}
		}
		else if (should_expand_token(v, va))
		{
			init_first_last(v, va, &static_buffer);
			if (v->ambiguous)
				ambiguous = false;
			if (va->var->wait_more_args)
			{
				if (v->i != -10)
				{
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
					static_buffer = v->bib[v->t];
				}
			}
			else
			{
				while (v->bib[v->i])
					add_expanded_token(v, &va->var->tokens, v->bib[v->i++], va);
				static_buffer = NULL;
			}		
		}
		else
		{
			if(va->var->wait_more_args)
				static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
			else 
			{
				if (!(QUOTE))
					v->new_token->need_expand = true;
				if (v->buff)
					static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
				v->buff = static_buffer;
				if (ambiguous)
					v->ambiguous = true;
				ambiguous = false;
				add_token(v, &va->var->tokens);
				static_buffer = NULL;
				QUOTE = false;
			}
		}
	}
	else
	{
		if (should_expand_token(v, va) && ambiguous_red && va->var->state == UNQUOTED)
		{
			v->expanded_value = expand_env_vars(v->new_buff, va);
			if (ft_strcmp(v->expanded_value, "") != 0)
			{
				v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
			}
			v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
			while (v->bib[v->i])
				v->i++;
			if (v->i > 1)
				ambiguous = true;
			if (va->var->wait_more_args && ambiguous)
			{
				// printf("111111\n");
				static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
				return (1);
			}
			else if (va->var->wait_more_args && v->i == 0)
			{
				// printf("222222\n");
				static_buffer = ft_strdup("", &va->allocs, P_GARBAGE);
				return (1);
			}
			else if (va->var->wait_more_args && !ambiguous && v->i == 1)
			{
				// printf("333333\n");
				if (ft_isspace(v->last))
				{
					add_expanded_token(v, &va->var->tokens, v->bib[0], va);
					static_buffer = NULL;
				}
				else
				{
					static_buffer = ft_strdup(v->bib[0], &va->allocs, P_GARBAGE);
				}
				return (1);
			}
			else if (!va->var->wait_more_args && ambiguous)
			{
				// printf("4444444\n");
				v->buff = v->new_buff;
				if (ambiguous)
					v->ambiguous = true;
				ambiguous = false;
				add_anbiguous_token(v, &va->var->tokens);
				return (1);
			}
			else if (!va->var->wait_more_args && !ambiguous && v->i == 0)
			{
				// printf("555555\n");
				v->buff = v->new_buff;
				add_anbiguous_token(v, &va->var->tokens);
				return (1);
			}
			else if (!va->var->wait_more_args && !ambiguous && v->i == 1)
			{
				// printf("666666666\n");
				add_expanded_token(v, &va->var->tokens, v->bib[0], va);
				static_buffer = NULL;
				return (1);
			}
		}
		else if (should_expand_token(v, va))
		{
			v->expanded_value = expand_env_vars(v->new_buff, va);
			if (ft_strcmp(v->expanded_value, "") != 0)
				v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
			v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
			if (va->var->wait_more_args)
				p_ex_without_buffer_fill(v, va, &static_buffer);
			else
			{
				v->s = -1;
				while (v->bib[++v->s])
					add_expanded_token(v, &va->var->tokens, v->bib[v->s], va);
				static_buffer = NULL;
			}
		}
		else
		{
			if (va->var->wait_more_args)
				static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
			else{
				if (!(QUOTE))
					v->new_token->need_expand = true;
				v->buff = v->new_buff;
				add_token(v, &va->var->tokens);
				v->buff = NULL;
				static_buffer = NULL;
			}
			QUOTE = false;
		}

	}



	// if(v->buff)
	// 	p_with_buffer(v, va, &static_buffer, &QUOTE);
	// else
	// 	p_without_buffer(v, va, &static_buffer, &QUOTE);
	return 1;
}
