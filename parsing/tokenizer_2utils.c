#include "he.h"


	
int add_token_with_type(t_sp_var *va)
{

	static char *static_buffer = NULL;
	static bool QUOTE = false;
	bool need_exand = false;
	t_v *v = mmallocc(sizeof(t_v), &va->allocs, P_GARBAGE);
	if (!v)
		return 0;
	int a = -1;


	init_variable(v);
	if (va->var->buffer[0] == '\0')
		return (0);
	if (va->var->state == UNQUOTED && !QUOTE)
		QUOTE = false; 
	else
		QUOTE = true;
	if (va->var->wait_more_args && static_buffer)
	{
		v->buff = static_buffer;
		static_buffer = NULL;
	}
	
	if (!ft_fill_1(va))
		return (0);//syntax error
	if (!ft_fill_2(v, va))
		return (0);//malloc failed
	v->new_token = create_new_token(&v->new_token, v->new_buff , va);
	       
	
	if (!v->new_token)
	{
		free(v->new_buff);
		return (0);//malloc failed
	}
	red_pip_txt(v, &va->var->state);
	// printf("[%d]\n",v->new_token->type);
	t_token *prev_token = NULL;

// Safely find the last token in the existing list
	if (va->var->tokens) {
    	prev_token = va->var->tokens;
    	while (prev_token->next)
       		prev_token = prev_token->next;
	}

// Only expand if the previous token is NOT "<<"
	if (!(prev_token && ft_strcmp(prev_token->value, "<<") == 0) && need_expandd(v->new_buff) && !va->var->wait_more_args && !static_buffer)
	{
    	char *expanded_value = expand_env_vars(v->new_buff, va);
		// free(v->new_buff);//
    	v->new_buff = expanded_value;
		char **bib = ft_split(v->new_buff,' ', &va->allocs);
		//append bib to tokens arg
		int s=-1;
		while (bib[++s])
			add_expanded_token(v, &va->var->tokens, bib[s], va);
		s=-1;
        // while (bib[++s])
        //     free(bib[s]);
        // free(bib);
		a = 10;
	}
		// printf("aaaaaaaa\n");

	// expand_variables(v->new_token, state);
	// add_token(v, tokens, wait_more_args);
	if (a != 10)
	{
		// printf("aaaaa\n");
		if (!(prev_token && ft_strcmp(prev_token->value, "<<") == 0) && need_expandd(v->new_buff))
		{
    		char *expanded_value = expand_env_vars(v->new_buff, va);
			// free(v->new_buff);
    		v->new_buff = expanded_value;
		}
		if (va->var->wait_more_args) {
			// if (*state != UNQUOTED)
			// 	v->quote = true;
        	if (v->buff) {
				// char *tmp = static_buffer;
           		static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
            	// free(v->buff);
        	}else {
            	static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
        	}
        	// free(v->new_token);
    	} else
		{
		// if (*state != UNQUOTED)
		// 	v->quote = true;
			if (!QUOTE)
				v->new_token->need_expand = true;
			if (!static_buffer)
			{
			// printf("\n[%s]\n\n",static_buffer);
				v->buff = v->new_buff;
        		add_token(v, &va->var->tokens);
				QUOTE = false;
			}else
			{
				char *joined = ft_strjoin(static_buffer, v->new_buff, &va->allocs);
				v->buff = joined;
				// free(v->new_buff);
			
				add_token(v, &va->var->tokens);
				// free(static_buffer);
				static_buffer = NULL;
				QUOTE = false;
			}
    	}	
	}
	return 1;
}
