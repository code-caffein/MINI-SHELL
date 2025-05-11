#include "he.h"


static void init_var(t_var *v, char *line)
{
	v->i = 0;
	v->j = -1;
	v->state = UNQUOTED;
	v->has_heredoc = false;
	v->wait_more_args = false;
	v->buffer = malloc(sizeof(char) * (ft_strlen(line) + 1));
if (!v->buffer)
    return;
	v->buffer[0] = '\0';
	v->tokens = NULL;
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
	// t_token *prev;
	int a = 0;

	detect_file_tokens(&v->tokens);
	validate_syntax(&v->tokens);
	curr = v->tokens;
	// prev = NULL;
	char *tmp_err;
	while (curr)
	{
		// printf("[%s]\n",curr->value);
		if (curr->syn_err)
		{
			has_syntax_error = true;
			tmp_err = curr->value;
			// printf("[%s]\n",curr->value);
			if (check_red(curr->value))
				a = 1;
			else
				a = 2;
			break;
		}
		if (curr->heredoc)
		{
			v->has_heredoc = true;
			// break;
		}
		// prev = curr;
		curr = curr->next;
	}
	if (has_syntax_error && v->has_heredoc)
	{
		// printf("syntax error near unexpected token \"%s\"\n", tmp_err);
		if (a == 1)
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		else if (a == 2)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			write(2, &tmp_err[0], 1);
			write(2, &tmp_err[1], 1);
			ft_putstr_fd("'\n", 2);
		}
		// free (tmp_err);
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
		// printf("syntax error near unexpected token \"%c\"\n", tmp_err[0]);
		free_token_list(&v->tokens);
		// free (tmp_err);
		return (NULL);//exit !!!!!! 
	}
	if (v->state != UNQUOTED)
	{
		free_token_list(&v->tokens);
		printf("syntax error quot:\n");
		return (NULL);
	}
	// if (has_syntax_error)
	// 	return (v->tokens);
	return (v->tokens);
}
//v->state == UNQUOTED && !is_token_separator(v->c) && (line[v->j + 1] == '\'' || line[v->j + 1] == '"' || !is_token_separator(line[v->j+1])))
t_token *tokenize_input(t_sp_var *va)
{
	va->var =malloc(sizeof(t_var));

	init_var(va->var, va->line);

	while (va->line[++va->var->j])
	{
		va->var->c = va->line[va->var->j];
		if (va->var->state == UNQUOTED && (va->var->c == '\'' || va->var->c == '"'))
			first_condtion(va->var, va->env, va->status);
		else if ((va->var->state == SINGLE_QUOTED && va->var->c == '\'' && (va->line[va->var->j + 1] != '\'' && va->line[va->var->j  + 1] != '"' && (is_token_separator(va->line[va->var->j + 1]) || !va->line[va->var->j + 1]))) || 
                (va->var->state == DOUBLE_QUOTED && va->var->c == '"'  && (va->line[va->var->j + 1] != '\'' && va->line[va->var->j  + 1] != '"' && (is_token_separator(va->line[va->var->j + 1]) || !va->line[va->var->j + 1]))))
		{
			va->var->wait_more_args = false;
			if (!second_condition(va->var, va->env, va->status))
				break;
		}
		else if ((va->var->state == SINGLE_QUOTED && va->var->c == '\'' && va->line[va->var->j + 1] && (va->line[va->var->j + 1] == '\'' || va->line[va->var->j + 1] == '"' || !is_token_separator(va->line[va->var->j+1]))) || 
				(va->var->state == DOUBLE_QUOTED && va->var->c == '"' && va->line[va->var->j + 1] && (va->line[va->var->j + 1] == '\'' || va->line[va->var->j + 1] == '"' || !is_token_separator(va->line[va->var->j+1]))))
		{
			if (!third_condition(va->var, va->env, va->status))
				break;
		}
		else if (va->var->state == UNQUOTED && (ft_strchr("|<>", va->var->c) || (va->var->buffer[0] == '<' || va->var->buffer[0] == '>' ||va->var->buffer[0] == '|')))
		{
			// printf("-|%c|-\n",v->buffer[0]);
			if (!Fourth_condition(va->var, va->line, va->env, va->status))
				break;
		}
		else if (va->var->state == UNQUOTED && ft_isspace(va->var->c))
		{
			if (!fifth_condition(va->var, va->env, va->status))
				break;
			continue;
		}
		else
		{
			va->var->buffer[va->var->i++] = va->var->c;
			// printf("____|%c|___\n",v->c);
		}
	}
	if (!sixth_condition(va->var, va->env, va->status))
	{
		free_token_list(&va->var->tokens);
		return(free(va->var),NULL);
	}
	free(va->var->buffer);
	t_token *result = fill_tokenize(va->var);
	free(va->var);
	return (result);
}
// int main(void)
// {
//     char *input;

//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input)
//         {
//             printf("exit\n");
//             break;
//         }
		
//         if (*input != '\0')
//             add_history(input);
// 		tokenize_input(input);
//         free(input);
//     }

//     return 0;
// }