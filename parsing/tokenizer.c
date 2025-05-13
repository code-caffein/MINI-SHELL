#include "he.h"


static void init_var(t_sp_var *va)
{
	va->var->i = 0;
	va->var->j = -1;
	va->var->state = UNQUOTED;
	va->var->has_heredoc = false;
	va->var->wait_more_args = false;
	va->var->buffer = mmallocc(sizeof(char) * (ft_strlen(va->line) + 1), &va->allocs, P_GARBAGE);
if (!va->var->buffer)
    return;
	va->var->buffer[0] = '\0';
	va->var->tokens = NULL;
}
int check_red(char *s)
{
	if (ft_strcmp(s,"<") == 0 || ft_strcmp(s,"<<") == 0 || ft_strcmp(s,">") == 0 || ft_strcmp(s,">>") == 0 )
		return 1;
	return 0;
}

t_token *fill_tokenize(t_var *v)
{
	bool has_syntax_error = false;
	t_token *curr;
	int a = 0;

	detect_file_tokens(&v->tokens);
	validate_syntax(&v->tokens);
	curr = v->tokens;
	char *tmp_err;
	while (curr)
	{
		if (curr->syn_err)
		{
			has_syntax_error = true;
			tmp_err = curr->value;
			if (check_red(curr->value))
				a = 1;
			else
				a = 2;
			break;
		}
		if (curr->heredoc)
			v->has_heredoc = true;
		curr = curr->next;
	}
	if (has_syntax_error && v->has_heredoc)
	{
		if (a == 1)
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		else if (a == 2)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			write(2, &tmp_err[0], 1);
			write(2, &tmp_err[1], 1);
			ft_putstr_fd("'\n", 2);
		}
		return (v->tokens);
	}

	if (has_syntax_error && !v->has_heredoc)
	{
		if (a == 1)
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		else if (a == 2)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			write(2, &tmp_err[0], 1);
			write(2, &tmp_err[1], 1);
			ft_putstr_fd("'\n", 2);
		}
		return (NULL);
	}
	if (v->state != UNQUOTED)
	{
		printf("syntax error quot:\n");
		return (NULL);
	}
	return (v->tokens);
}

t_token *tokenize_input(t_sp_var *va)
{
	va->var =mmallocc(sizeof(t_var), &va->allocs, P_GARBAGE);

	init_var(va);

	while (va->line[++va->var->j])
	{
		va->var->c = va->line[va->var->j];
		if (va->var->state == UNQUOTED && (va->var->c == '\'' || va->var->c == '"'))
			first_condtion(va);
		else if ((va->var->state == SINGLE_QUOTED && va->var->c == '\'' && (va->line[va->var->j + 1] != '\'' && va->line[va->var->j  + 1] != '"' && (is_token_separator(va->line[va->var->j + 1]) || !va->line[va->var->j + 1]))) || 
                (va->var->state == DOUBLE_QUOTED && va->var->c == '"'  && (va->line[va->var->j + 1] != '\'' && va->line[va->var->j  + 1] != '"' && (is_token_separator(va->line[va->var->j + 1]) || !va->line[va->var->j + 1]))))
		{
			va->var->wait_more_args = false;
			if (!second_condition(va))
				break;
		}
		else if ((va->var->state == SINGLE_QUOTED && va->var->c == '\'' && va->line[va->var->j + 1] && (va->line[va->var->j + 1] == '\'' || va->line[va->var->j + 1] == '"' || !is_token_separator(va->line[va->var->j+1]))) || 
				(va->var->state == DOUBLE_QUOTED && va->var->c == '"' && va->line[va->var->j + 1] && (va->line[va->var->j + 1] == '\'' || va->line[va->var->j + 1] == '"' || !is_token_separator(va->line[va->var->j+1]))))
		{
			if (!third_condition(va))
				break;
		}
		else if (va->var->state == UNQUOTED && (ft_strchr("|<>", va->var->c) || (va->var->buffer[0] == '<' || va->var->buffer[0] == '>' ||va->var->buffer[0] == '|')))
		{
			if (!Fourth_condition(va))
				break;
		}
		else if (va->var->state == UNQUOTED && ft_isspace(va->var->c))
		{
			if (!fifth_condition(va))
				break;
			continue;
		}
		else
			va->var->buffer[va->var->i++] = va->var->c;
	}
	if (!sixth_condition(va))
		return(NULL);
	t_token *result = fill_tokenize(va->var);
	return (result);
}
