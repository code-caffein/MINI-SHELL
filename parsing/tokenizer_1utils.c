#include "parsing.h"

void	init_var(t_sp_var *va)
{
	va->var->i = 0;
	va->var->j = -1;
	va->var->a = -1;
	va->var->p = mmallocc(sizeof(int), &va->allocs, P_GARBAGE);
    if (!va->var->p)
	{
        return;
	}
	*va->var->p = 0;
	va->var->state = UNQUOTED;
	va->var->has_heredoc = false;
	va->var->wait_more_args = false;
	va->var->buffer = mmallocc(sizeof(char) * (ft_strlen(va->line) + 1), &va->allocs, P_GARBAGE);
	if (!va->var->buffer)
	return;
	va->var->buffer[0] = '\0';
	va->var->tokens = NULL;
}

bool	fill_tken_part1(t_var *v)
{
	t_token	*curr;

	detect_file_tokens(&v->tokens);
	validate_syntax(&v->tokens);
	curr = v->tokens;
	v->has_heredoc = false;
	while (curr)
	{
		if (curr->syn_err)
		{
			v->tmp_err = curr->value;
			if (!curr->newline)
				*v->p = 1;
			else
				*v->p = 2;
			return (true);
		}
		if (curr->heredoc)
			v->has_heredoc = true;
		curr = curr->next;
	}
	return (false);
}

t_token *fill_tken_part2(t_var *v)
{
	if (*(v->p) == 1)
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
	else if (*(v->p) == 2)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		write(2, &v->tmp_err[0], 1);
		write(2, &v->tmp_err[1], 1);
		ft_putstr_fd("'\n", 2);
	}
	return (v->tokens);
}
t_token *fill_tken_part3(t_var *v)
{
	if (*(v->p) == 1)
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
	else if (*(v->p) == 2)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		write(2, &v->tmp_err[0], 1);
		write(2, &v->tmp_err[1], 1);
		ft_putstr_fd("'\n", 2);
	}
	return (NULL);
}