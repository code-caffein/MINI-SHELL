#include "he.h"


static void init_var(t_var *v)
{
	v->i = 0;
	v->j = -1;
	v->state = UNQUOTED;
	v->has_heredoc = false;
	v->wait_more_args = false;
	v->buffer[0] = '\0';
	v->tokens = NULL;
}

t_token *fill_tokenize(t_var *v)
{
	bool has_syntax_error;
	t_token *curr;
	t_token *prev;

	detect_file_tokens(&v->tokens);
	validate_syntax(&v->tokens);
	curr = v->tokens;
	prev = NULL;
	while (curr)
	{
		if (curr->heredoc)
		{
			v->has_heredoc = true;
			break;
		}
		if (curr->syn_err)
		{
			has_syntax_error = true;
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	if (prev && prev->syn_err && v->has_heredoc)
		return (v->tokens);
	if (prev && prev->syn_err && !v->has_heredoc)
	{
		free_token_list(v->tokens);
		return (NULL);//exit !!!!!! 
	}
	if (v->i > 0 && v->state != UNQUOTED)
	{
		free_token_list(v->tokens);
		return (NULL);
	}
	if (prev && !prev->syn_err)
		return (v->tokens);
	return (NULL);
}

t_token *tokenize_input(char *line)
{
	t_var *v;

	init_var(v);
	while (line[++v->j])
	{
		v->c = line[v->j];
		if (v->state == UNQUOTED && (v->c == '\'' || v->c == '"'))
			first_condtion(v);
		else if ((v->state == SINGLE_QUOTED && v->c == '\'' && (line[v->j + 1] != '\'' && line[v->j  + 1] != '"' && is_token_separator(line[v->j + 1]) || !line[v->j + 1])) || 
                (v->state == DOUBLE_QUOTED && v->c == '"'  && (line[v->j + 1] != '\'' && line[v->j  + 1] != '"' && is_token_separator(line[v->j + 1]) || !line[v->j + 1])))
		{
			if (!second_condition(v))
				break;
		}
		else if ((v->state == SINGLE_QUOTED && v->c == '\'' && line[v->j + 1] && (line[v->j + 1] == '\'' || line[v->j + 1] == '"' || !is_token_separator(line[v->j+1]))) || 
				(v->state == DOUBLE_QUOTED && v->c == '"' && line[v->j + 1] && (line[v->j + 1] == '\'' || line[v->j + 1] == '"' || !is_token_separator(line[v->j+1]))))
		{
			if (!third_condition(v))
				break;
		}
		else if (v->state == UNQUOTED && (ft_strchr("|<>", v->c) || v->buffer[0] == '|' || v->buffer[0] == '>' || v->buffer[0] == '<'))
			if (!Fourth_condition(v, line))
				break;
		else if (v->state == UNQUOTED && ft_isspace(v->c))
			if (!fifth_condition(v))
				break;
		else
			v->buffer[v->i++] = v->c;
	}
	if (!sixth_condition(v))
	{
		free_token_list(v->tokens);
		return NULL;
	}
	return (fill_tokenize(v));
}
