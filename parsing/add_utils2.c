#include "parsing.h"
t_v *init_token_vars(t_sp_var *va, bool *quote)
{
    t_v *v = mmallocc(sizeof(t_v), &va->allocs, P_GARBAGE);
    if (!v)
        return NULL;
    
    init_variable(v, va->var->tokens);
    update_quote_state(va, quote);
    v->quote = *quote;
    return v;
}


int prepare_token(t_sp_var *va, t_v *v)
{
    if (!ft_fill_1(va))
        return 0; // syntax error
    
    if (!ft_fill_2(v, va))
        return 0; // malloc failed
    
    v->new_token = create_new_token(&v->new_token, v->new_buff, va);
    if (!v->new_token)
    {
        return 0; // malloc failed
    }
	v->new_token->ambiguous = false;
    v->new_token->need_expand = false;
    red_pip_txt(v, &va->var->state);
    
    return 1;
}