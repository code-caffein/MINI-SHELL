#include "he.h"

int ft_fill_1(t_token **tokens, char *buffer, t_quote_state *state)
{
	if (buffer[0] == '>' || buffer[0] == '<' || buffer[0] == '|')
	{
		if (!check_operator_validity(buffer, state))
		{
			add_error_token(tokens, buffer);
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
    if (!v)
        return;
        
    v->new_token = NULL;
    v->current = NULL;
    v->buff = NULL;
    v->joined = NULL;
    v->new_buff = NULL;
	// v->quote = false;
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
	if (v->buff) {
            char *joined = ft_strjoin(v->buff, v->new_buff);
            free(v->buff);
            free(v->new_buff);
            v->buff = joined;
    } else{
            v->buff = ft_strdup(v->new_buff);
        }
		// printf("buff = %s\n", v->buff);
        free(v->new_token->value);
        free(v->new_token);
}
void add_expanded_token(t_v *v, t_token **tokens, char *expanded)
{
	t_token *new_token;
	char *tmp = ft_strdup(expanded);
	new_token = create_new_token(&new_token, tmp);
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

int add_token_with_type(t_var *va, t_env *env, int status)
{

	static char *static_buffer = NULL;
	static bool QUOTE = false;
	bool need_exand = false;
	t_v *v = malloc(sizeof(t_v));
	if (!v)
		return 0;
	int a = -1;
	printf("buffer = [%s]\n", va->buffer);
	// (*tokens)->need_expand = malloc(sizeof(bool));
	// if (state == UNQUOTED)
	// 	(*tokens)->need_expand = true;
	// else
	// 	(*tokens)->need_expand = false;
	init_variable(v);
	if (va->buffer[0] == '\0')
		return (free(v),0);
	if (va->state == UNQUOTED && !QUOTE)
		QUOTE = false; 
	else
		QUOTE = true;
	if (va->wait_more_args && static_buffer)
	{
		v->buff = static_buffer;
		static_buffer = NULL;
	}
	
	if (!ft_fill_1(&va->tokens, va->buffer, &va->state))
		return (free(v),0);//syntax error
	if (!ft_fill_2(v, va->buffer, &va->state))
		return (free(v),0);//malloc failed
	v->new_token = create_new_token(&v->new_token, v->new_buff);
	
	
	if (!v->new_token)
	{
		free(v->new_buff);
		return (free(v),0);//malloc failed
	}
	red_pip_txt(v, &va->state);
	// printf("[%d]\n",v->new_token->type);
	t_token *prev_token = NULL;

// Safely find the last token in the existing list
	if (va->tokens) {
    	prev_token = va->tokens;
    	while (prev_token->next)
       		prev_token = prev_token->next;
	}

// Only expand if the previous token is NOT "<<"
	if (!(prev_token && ft_strcmp(prev_token->value, "<<") == 0) && need_expandd(v->new_buff) && !va->wait_more_args && !static_buffer)
	{
    	char *expanded_value = expand_env_vars(v->new_buff, &va->state, env, status);
    	v->new_buff = expanded_value;
		char **bib = ft_split(v->new_buff,' ');
		//append bib to tokens arg
		int s=-1;
		while (bib[++s])
			add_expanded_token(v, &va->tokens, bib[s]);
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
    		char *expanded_value = expand_env_vars(v->new_buff, &va->state, env, status);
    		v->new_buff = expanded_value;
		}
		if (va->wait_more_args) {
			// if (*state != UNQUOTED)
			// 	v->quote = true;
        	if (v->buff) {
				// char *tmp = static_buffer;
           		static_buffer = ft_strjoin(v->buff, v->new_buff);
            	free(v->buff);
        	}else {
            	static_buffer = ft_strdup(v->new_buff);
        	}
        	free(v->new_token);
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
        		add_token(v, &va->tokens);
				QUOTE = false;
			}else
			{
				char *joined = ft_strjoin(static_buffer, v->new_buff);
				v->buff = joined;
				free(v->new_buff);
			
				add_token(v, &va->tokens);
				static_buffer = NULL;
				QUOTE = false;
			}
    	}	
	}
	free(v);	
	return 1;
}
