#include "he.h"


static void	p_ex_without_buffer_fill(t_v *v, t_sp_var *va, char **static_buffer)
{
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
	
int add_token_with_type(t_sp_var *va)
{
	static char	*static_buffer = NULL;
	static bool	QUOTE = false;
	t_v *v;
    
	v = init_token_vars(va, &QUOTE);
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
		p_with_buffer(v, va, &static_buffer, &QUOTE);
	else
		p_without_buffer(v, va, &static_buffer, &QUOTE);
	return 1;
}
