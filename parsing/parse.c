
#include "he.h"

int g_signal_pid;


void	init_handle_red_var(t_h_red_v *hrv, t_token *token, int ss)
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
	if (token && token->next)
		hrv->file_token = token->next;
	if(hrv->file_token->need_expand)
		hrv->i = 1;
	hrv->redir = NULL;
	hrv->current = NULL;
}

int red_type(char *value)
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


// int init_heredoc_buffer(t_sp_var *va)
// {
// 	va->hrv->capacity = 10;
// 	va->hrv->bib = mmallocc(sizeof(char *) * va->hrv->capacity, &va->allocs, P_GARBAGE);
// 	if (!va->hrv->bib)
// 		return errno;
// 	return 0;
// }

// int read_heredoc_line(t_sp_var *va)
// {
// 	g_signal_pid = 2;
// 	if (g_signal_pid == -1)
// 		return -2;
	
// 	write(1, "> ", 2);
// 	va->hrv->n = heredoc_readline(&va->hrv->line, va);
	
// 	if (!va->hrv->line || va->hrv->n == -2 || va->hrv->n == 0)
// 		return -2;
		
// 	if (va->hrv->n > 0 && va->hrv->line[va->hrv->n - 1] == '\n')
// 		va->hrv->line[va->hrv->n-1] = '\0';
		
// 	if (ft_strcmp(va->hrv->line, va->hrv->file_token->value) == 0)
// 		return 1;
	
// 	return 0;
// }

// void expand_heredoc_line(t_sp_var *va)
// {
// 	if (va->hrv->i == 1)
// 	{
// 		va->var->state = UNQUOTED;
// 		va->hrv->tmp = expand_env_vars(va->hrv->line, va);
// 		va->hrv->line = va->hrv->tmp;
// 	}
// }

// /**
//  * Resize heredoc buffer if needed
//  */
// int resize_heredoc_buffer(t_sp_var *va)
// {
// 	if (va->hrv->in >= va->hrv->capacity - 1)
// 	{
// 		va->hrv->capacity *= 2;
// 		va->hrv->new_bib = mmallocc(sizeof(char *) * va->hrv->capacity, &va->allocs, P_GARBAGE);
// 		if (!va->hrv->new_bib)
// 			return errno;
			
// 		va->hrv->j = -1;
// 		while (++va->hrv->j < va->hrv->in)
// 			va->hrv->new_bib[va->hrv->j] = va->hrv->bib[va->hrv->j];
			
// 		va->hrv->bib = va->hrv->new_bib;
// 	}
// 	return 0;
// }

// /**
//  * Processes the heredoc content into a temporary file
//  */
// int heredoc_to_temp_file(t_sp_var *va)
// {
// 	g_signal_pid = 0;
// 	va->hrv->bib[va->hrv->in] = NULL;

// 	va->hrv->fd = open("t_m_p_f_i_l_e", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (va->hrv->fd < 0)
// 		return errno;
		
// 	va->hrv->j = -1;
// 	while (++va->hrv->j < va->hrv->in)
// 	{
// 		write(va->hrv->fd, va->hrv->bib[va->hrv->j], ft_strlen(va->hrv->bib[va->hrv->j]));
// 		write(va->hrv->fd, "\n", 1);
// 	}
	
// 	close(va->hrv->fd);
// 	va->hrv->redir->fd = open("t_m_p_f_i_l_e", O_RDONLY);
// 	return 0;
// }

// /**
//  * Main heredoc processing function
//  */
// int process_heredoc(t_sp_var *va)
// {
// 	int result;
	
// 	result = init_heredoc_buffer(va);
// 	if (result != 0)
// 		return result;
// 	while (1)
// 	{
// 		result = read_heredoc_line(va);
// 		if (result != 0)
// 			break; // Either error or found delimiter
		
// 		expand_heredoc_line(va);
		
// 		result = resize_heredoc_buffer(va);
// 		if (result != 0)
// 			return result;
		
// 		va->hrv->bib[va->hrv->in++] = va->hrv->line;
// 	}
	
// 	if (result < 0)
// 		return result;
		
// 	return heredoc_to_temp_file(va);
// }

/**
 * Main handler for heredoc and input redirection
 */
int handle_red_if1(t_cmd *cmd, t_sp_var *va)
{
	int result = 0;
	
	if (va->hrv->redir_type == REDIR_HEREDOC)
		result = process_heredoc(va);
	else if (va->hrv->redir_type == REDIR_IN && va->hrv->ss != 0)
		va->hrv->redir->fd = open(va->hrv->redir->file, O_RDONLY);
	return result;
}


int handle_redirection(t_cmd *cmd, t_token *token, t_sp_var *va, int ss)
{
	va->hrv = mmallocc(sizeof(t_h_red_v), &va->allocs, P_GARBAGE);
	if (!va->hrv || !cmd || !token || !token->next)
		return (0);

	init_handle_red_var(va->hrv, token, ss);


    va->hrv->redir_type = red_type(token->value);

    va->hrv->redir = create_redirection(va->hrv->file_token->value, va->hrv->redir_type, va);

    if (va->hrv->redir_type == REDIR_IN || va->hrv->redir_type == REDIR_HEREDOC)
	{
		// if (va->hrv->redir_type == REDIR_HEREDOC)
		// {
		// 	va->hrv->capacity = 10;
		// 	va->hrv->bib = mmallocc(sizeof(char *) * va->hrv->capacity, &va->allocs, P_GARBAGE); // Allocate initial memory
		// 	if (!va->hrv->bib)
    	// 		return errno;
			
		// 	while (1)
		// 	{
		// 		g_signal_pid = 2;
		// 		if (g_signal_pid == -1)
		// 			return -2;
    	// 		write(1, "> ", 2);
   		// 		va->hrv->n = heredoc_readline(&va->hrv->line, va);
    	// 		if (!va->hrv->line || va->hrv->n == -2 || va->hrv->n == 0)
        // 			return -2;
		// 		if (va->hrv->n > 0 && va->hrv->line[va->hrv->n - 1] == '\n')
    	// 			va->hrv->line[va->hrv->n-1] = '\0';
    	// 		if (ft_strcmp(va->hrv->line, va->hrv->file_token->value) == 0)
       	// 			break;
    	// 		if (va->hrv->i == 1)
		// 		{
        // 			va->var->state = UNQUOTED;
        // 			va->hrv->tmp = expand_env_vars(va->hrv->line, va);
        // 			va->hrv->line = va->hrv->tmp;
    	// 		}
    	// 		if (va->hrv->in >= va->hrv->capacity - 1)
		// 		{
        // 			va->hrv->capacity *= 2;
        // 			va->hrv->new_bib = mmallocc(sizeof(char *) * va->hrv->capacity, &va->allocs, P_GARBAGE);
        // 			if (!va->hrv->new_bib)
        //     			return errno;
		// 			va->hrv->j = -1;
		// 			while (++va->hrv->j < va->hrv->in)
		// 				va->hrv->new_bib[va->hrv->j] = va->hrv->bib[va->hrv->j];
        // 			va->hrv->bib = va->hrv->new_bib;
    	// 		}
    	// 		va->hrv->bib[va->hrv->in++] = va->hrv->line;
		// 	}
		// 	g_signal_pid = 0;
		// 	va->hrv->bib[va->hrv->in] = NULL;

		// 	va->hrv->fd = open("t_m_p_f_i_l_e", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   		// 	if (va->hrv->fd < 0)
    	// 	{
       	// 		return errno;
		// 	}
		// 	va->hrv->j = -1;
		// 	while (++va->hrv->j < va->hrv->in)
		// 	{
		// 		write(va->hrv->fd, va->hrv->bib[va->hrv->j], ft_strlen(va->hrv->bib[va->hrv->j]));
		// 		write(va->hrv->fd, "\n", 1);
		// 	}
    	// 	close(va->hrv->fd);
    	// 	va->hrv->redir->fd = open("t_m_p_f_i_l_e", O_RDONLY);
		// }
		// else if (REDIR_IN && ss != 0)//input!!!
		// 	va->hrv->redir->fd = open(va->hrv->redir->file, O_RDONLY);
		int a = handle_red_if1(cmd, va);
		if (a!=0)
			return a;
		/////////////
		if (va->hrv->redir->fd < 0 && ss != 0)
		{
        	return errno;
    	}
        if (!cmd->in)
            cmd->in = va->hrv->redir;
		else
		{
			va->hrv->current = cmd->in;
            while (va->hrv->current->next)
                va->hrv->current = va->hrv->current->next;
            va->hrv->current->next = va->hrv->redir;
        }
    } else if ((va->hrv->redir_type == REDIR_OUT || va->hrv->redir_type == REDIR_APPEND) && ss != 0)
	{
		if (va->hrv->redir_type == REDIR_APPEND)
			va->hrv->redir->fd = open(va->hrv->redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			va->hrv->redir->fd = open(va->hrv->redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (va->hrv->redir->fd < 0)
		{
        	return errno;
    	}
        if (!cmd->out)
            cmd->out = va->hrv->redir;
        else
		{
			va->hrv->current = cmd->out;
            while (va->hrv->current->next)
                va->hrv->current = va->hrv->current->next;
            va->hrv->current->next = va->hrv->redir;
        }
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
    return va->vpt->commands;
}
