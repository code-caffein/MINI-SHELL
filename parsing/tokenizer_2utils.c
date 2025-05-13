#include "he.h"

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
void init_variable(t_v *v)
{
    if (!v)
        return;
        
    v->new_token = NULL;
    v->current = NULL;
    v->buff = NULL;
    v->joined = NULL;
    v->new_buff = NULL;
	// v->quote = false;
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

// void fill1_add_token(t_v *v)
// {
// 	if (v->buff) {
//             char *joined = ft_sttrjoin(v->buff, v->new_buff);
//             free(v->buff);
//             free(v->new_buff);
//             v->buff = joined;
//     } else{
//             v->buff = ft_sttrdup(v->new_buff);
//         }
// 		// printf("buff = %s\n", v->buff);
//         free(v->new_token->value);
//         free(v->new_token);
// }
void add_expanded_token(t_v *v, t_token **tokens, char *expanded, t_sp_var *va)
{
	t_token *new_token;
	char *tmp = ft_strdup(expanded, &va->allocs, P_GARBAGE);
	new_token = create_new_token(&new_token, tmp, va);
	if (new_token) {
        new_token->type = text; // Set the token type to text
    }
	v->new_token = new_token;
	// new_token->type = text;
	// printf("aaaaaaaaaaaaaa\n");
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
	// (void)wait_more_args;
	// if (wait_more_args)
	// 	fill1_add_token(v);
	// else
	// {
	// 	//  printf("Finalizing token - v->buff: %s, v->new_buff: %s\n", 
    //     //        v->buff ? v->buff : "NULL", 
    //     //        v->new_buff ? v->new_buff : "NULL");
    //     // Finalize token
    //     if (v->buff != NULL)
	// 	{
	// 		// printf("\n11111\n");
    //         char *final_value = ft_strjoin(v->buff, v->new_buff);
    //         free(v->buff);
    //         free(v->new_buff);
    //         v->new_token->value = final_value;
    //         v->buff = NULL;
    //     } else
    //         v->new_token->value = v->new_buff;
	// 	// printf("buff = %s\n", v->new_token->value);
	// printf("%s\n", v->buff);
		v->new_token->value = v->buff;
		// v->new_token->type = text;
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
	
int need_expandd(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int add_token_with_type(t_sp_var *va)
{

	static char *static_buffer = NULL;
	static bool QUOTE = false;
	bool need_exand = false;
	t_v *v = mmallocc(sizeof(t_v), &va->allocs, P_GARBAGE);
	if (!v)
		return 0;
	int a = -1;
	// printf("buffer = [%s]\n", va->buffer);
	// (*tokens)->need_expand = malloc(sizeof(bool));
	// if (state == UNQUOTED)
	// 	(*tokens)->need_expand = true;
	// else
	// 	(*tokens)->need_expand = false;
	// printf("[%s]\n",va->buffer);

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
	// if (v->buff)
	// 	free(v->buff);
	// if (v->new_buff)
	// 	free(v->new_buff);
	// if (v->joined)
	// 	free(v->joined);
	// free(v);	
	return 1;
}
