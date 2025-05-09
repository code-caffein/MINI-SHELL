#include "he.h"


static void init_var(t_var *v)
{
	v->i = 0;
	v->j = -1;
	v->state = UNQUOTED;
	v->has_heredoc = false;
	v->wait_more_args = false;
	v->buffer = malloc(sizeof(char) * 2000);
	if (!v->buffer)
		return ;
	v->buffer[0] = '\0';
	v->tokens = NULL;
}
int check_red(char *s)
{
	if (ft_strcmp(s,"<") == 0 || ft_strcmp(s,"<<") == 0 || ft_strcmp(s,">") == 0 || ft_strcmp(s,">>") == 0 || ft_strcmp(s,"|") == 0)
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
t_token *tokenize_input(char *line, t_env *env, int status)
{
	t_var *v = malloc(sizeof(t_var));
	if (!v)
		return (NULL);

	init_var(v);
	while (line[++v->j])
	{
		v->c = line[v->j];
		if (v->state == UNQUOTED && (v->c == '\'' || v->c == '"'))
			first_condtion(v, env, status);
		else if ((v->state == SINGLE_QUOTED && v->c == '\'' && (line[v->j + 1] != '\'' && line[v->j  + 1] != '"' && (is_token_separator(line[v->j + 1]) || !line[v->j + 1]))) || 
                (v->state == DOUBLE_QUOTED && v->c == '"'  && (line[v->j + 1] != '\'' && line[v->j  + 1] != '"' && (is_token_separator(line[v->j + 1]) || !line[v->j + 1]))))
		{
			v->wait_more_args = false;
			if (!second_condition(v, env, status))
				break;
		}
		else if ((v->state == SINGLE_QUOTED && v->c == '\'' && line[v->j + 1] && (line[v->j + 1] == '\'' || line[v->j + 1] == '"' || !is_token_separator(line[v->j+1]))) || 
				(v->state == DOUBLE_QUOTED && v->c == '"' && line[v->j + 1] && (line[v->j + 1] == '\'' || line[v->j + 1] == '"' || !is_token_separator(line[v->j+1]))))
		{
			if (!third_condition(v, env, status))
				break;
		}
		else if (v->state == UNQUOTED && (ft_strchr("|<>", v->c) || v->buffer[0] == '|' || v->buffer[0] == '>' || v->buffer[0] == '<'))
		{
			if (!Fourth_condition(v, line, env, status))
				break;
		}
		else if (v->state == UNQUOTED && ft_isspace(v->c))
		{
			if (!fifth_condition(v, env, status))
				break;
			continue;
		}
		else
			v->buffer[v->i++] = v->c;
	}
	if (!sixth_condition(v, env, status))
	{
		free_token_list(&v->tokens);
		return(free(v),NULL);
	}
	free(v->buffer);
	t_token *result = fill_tokenize(v);
	free(v);
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