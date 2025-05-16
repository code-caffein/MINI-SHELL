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
	if (va->var->state != UNQUOTED && (!QUOTE || QUOTE))
		QUOTE = true; 
	else if(va->var->state == UNQUOTED && !QUOTE)
		QUOTE = false;
	else if(va->var->state == UNQUOTED && QUOTE)
		QUOTE = true;
	
	v->quote = QUOTE;
	// printf("[      [%s]    ]\n",va->var->buffer);
	if (static_buffer)
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
		return (0);//malloc failed
	v->new_token->need_expand = false;

	red_pip_txt(v, &va->var->state);
	t_token *prev_token = NULL;


	if (va->var->tokens)
	{
    	prev_token = va->var->tokens;
    	while (prev_token->next)
       		prev_token = prev_token->next;
	}


	char first;
	char last;
	if(v->buff)
	{
		char *tmp;
		if (!(prev_token && ft_strcmp(prev_token->value, "<<") == 0) && need_expandd(v->new_buff, &va->var->state))
		{
			char *expanded_value = expand_env_vars(v->new_buff, va);
			if (ft_strcmp(expanded_value, "") != 0)
			{
				first = expanded_value[0];
				last = expanded_value[ft_strlen(expanded_value) - 1];
			}
			char **bib = ft_split(expanded_value,' ', &va->allocs);
			int i = 0;

			if (!ft_isspace(first))
			{
				static_buffer = ft_strjoin(v->buff, bib[i++], &va->allocs);
				add_expanded_token(v, &va->var->tokens, static_buffer, va);
			}
			else
				add_expanded_token(v, &va->var->tokens, v->buff, va);
			int s = i;
			if (va->var->wait_more_args)
			{
				while (bib[i])
					i++;
				int t = i - 1;
				i = s;
				while(bib[i])
				{
					if (i != t)
						add_expanded_token(v, &va->var->tokens, bib[i++], va);
					else
						break;
				}
				if (ft_isspace(last))
					add_expanded_token(v, &va->var->tokens, bib[t], va);
				else
					static_buffer = bib[t];
			}
			else
			{
				while (bib[i])
					add_expanded_token(v, &va->var->tokens, bib[i++], va);
				static_buffer = NULL;
				// QUOTE = false;
			}
		}
		else
		{
			if(va->var->wait_more_args)
				static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
			else 
			{
				if (!QUOTE)
					v->new_token->need_expand = true;
				static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
				v->buff = static_buffer;
				add_token(v,  &va->var->tokens);
				static_buffer = NULL;
				QUOTE = false;
			}
		}
	}
	else
	{
		if (!(prev_token && ft_strcmp(prev_token->value, "<<") == 0) && need_expandd(v->new_buff, &va->var->state))
		{
			char *expanded_value = expand_env_vars(v->new_buff, va);
			if (ft_strcmp(expanded_value, "") != 0)
				last = expanded_value[ft_strlen(expanded_value) - 1];
			char **bib = ft_split(expanded_value,' ', &va->allocs);
			if (va->var->wait_more_args)
			{
				int i = 0;
				while (bib[i])
					i++;
				int t = i - 1;
				i = 0;
				while(bib[i])
				{
					if (i != t)
						add_expanded_token(v, &va->var->tokens, bib[i++], va);
					else
						break;
				}
				if (ft_isspace(last))
					add_expanded_token(v, &va->var->tokens, bib[t], va);
				else
					static_buffer = bib[t];
				// v->quote = false;
			}
			else
			{
				int s=-1;
				while (bib[++s])
					add_expanded_token(v, &va->var->tokens, bib[s], va);
				static_buffer = NULL;
				// v->quote = false;
			}
		}
		else
		{
			if (va->var->wait_more_args)
				static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
			else{
				if (!QUOTE)
					v->new_token->need_expand = true;
				v->buff = v->new_buff;
				add_token(v,  &va->var->tokens);
				v->buff = NULL;
			}
			QUOTE = false;
		}
	}






	




















	// if (a != 10)
	// {
	// 	// printf("aaaaa\n");
	// 	if (!(prev_token && ft_strcmp(prev_token->value, "<<") == 0) && need_expandd(v->new_buff))
	// 	{
	// 		// printf("AAAAAA\n");
    // 		char *expanded_value = expand_env_vars(v->new_buff, va);
	// 		char **bib = ft_split(expanded_value,' ', &va->allocs);
	// 		int s=-1;
	// 		while (bib[++s])
	// 			add_expanded_token(v, &va->var->tokens, bib[s], va);
	// 		return 1;
    // 		// v->new_buff = expanded_value;
	// 	}
	// 	if (va->var->wait_more_args) {
	// 		// if (*state != UNQUOTED)
	// 		// 	v->quote = true;
    //     	if (v->buff) {
	// 			// char *tmp = static_buffer;
    //        		static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);

    //     	}else {
    //         	static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
    //     	}
    //     	// free(v->new_token);
    // 	} else
	// 	{



	// 		if (!QUOTE)
	// 			v->new_token->need_expand = true;
	// 		if (!static_buffer)
	// 		{
	// 		// printf("\n[%s]\n\n",static_buffer);
	// 			v->buff = v->new_buff;
    //     		add_token(v, &va->var->tokens);
	// 			QUOTE = false;
	// 		}else
	// 		{
	// 			char *joined = ft_strjoin(static_buffer, v->new_buff, &va->allocs);
	// 			v->buff = joined;
	// 			// free(v->new_buff);
			
	// 			add_token(v, &va->var->tokens);
	// 			// free(static_buffer);
	// 			static_buffer = NULL;
	// 			QUOTE = false;
	// 		}
    // 	}	
	// }
	return 1;
}
