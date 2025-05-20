
#include "parsing.h"

int	g_signal_pid;


static void	init_handle_red_var(t_h_red_v *hrv, t_token *token, int ss)
{
	hrv->i = 0;
	hrv->redir_type = 0;
	hrv->in = 0;
	hrv->capacity = 0;
	hrv->bib = NULL;
	hrv->n = 0;
	hrv->line = NULL;
	hrv->tmp = NULL;
	hrv->new_bib = NULL;
	hrv->j = 0;
	hrv->fd = 0;
	hrv->ss = ss;
	hrv->result = 0;
	if (token && token->next)
		hrv->file_token = token->next;
	if(hrv->file_token->need_expand)
		hrv->i = 1;
	hrv->redir = NULL;
	hrv->current = NULL;
}

int	red_type(char *value)
{
	if (ft_strcmp(value, "<") == 0)
		return (REDIR_IN);
	else if (ft_strcmp(value, ">") == 0)
		return (REDIR_OUT);
	else if (ft_strcmp(value, ">>") == 0)
		return (REDIR_APPEND);
	else if (ft_strcmp(value, "<<") == 0)
		return (REDIR_HEREDOC);
	return (-1);
}

int	handle_redirection(t_cmd *cmd, t_token *token, t_sp_var *va, int ss)
{
	va->hrv = mmallocc(sizeof(t_h_red_v), &va->allocs, P_GARBAGE);
	if (!va->hrv || !cmd || !token || !token->next)
		return (0);
	init_handle_red_var(va->hrv, token, ss);
	va->hrv->redir_type = red_type(token->value);
	va->hrv->redir = create_redirection(va->hrv->file_token->value, va->hrv->redir_type, va);
	if (va->hrv->redir_type == REDIR_IN || va->hrv->redir_type == REDIR_HEREDOC)
	{
		va->hrv->result = handle_red_if1(cmd, va);
		if (va->hrv->result != 0)
			return (va->hrv->result);
		va->hrv->result = handle_red_if2(cmd, va);
		if (va->hrv->result != 0)
			return (va->hrv->result);
	} else if ((va->hrv->redir_type == REDIR_OUT || va->hrv->redir_type == REDIR_APPEND) && ss != 0)
	{
		va->hrv->result = open_outfile(va);
		if (va->hrv->result != 0)
			return (va->hrv->result);
		add_red_file(cmd, va);
	}
	return 0;
}



t_cmd *parse_tokens(t_token *tokens, t_sp_var *va)
{
	va->vpt = mmallocc(sizeof(t_pt), &va->allocs, P_GARBAGE);
	if (!va->vpt)
		return NULL;
	init_vpt(tokens, va->vpt);
	if (!ft_detect_syn_err(tokens, va))
		return NULL;
	va->vpt->a = while_part(va);
	if (va->vpt->a == 0)
		return (NULL);
	if (va->vpt->a == 10)
	{
		va->status = 999;
		return (NULL);
	}
	return (va->vpt->commands);
}
