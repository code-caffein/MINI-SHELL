#include "parsing.h"

int g_signal_pid;
int process_heredoc(t_sp_var *va)
{
	int result;
	// int tmp;
	
	result = init_heredoc_buffer(va);
	if (result != 0)
		return result;
	g_signal_pid = 2;  // Set heredoc mode
    signals(1);

	while (1)
	{
		result = read_heredoc_line(va);
		if (result == -2)
		{
            signals(0); 
            g_signal_pid = 0;
            return -2;
        }
		if (result != 0)
			break;
		
		expand_heredoc_line(va);
		
		result = resize_heredoc_buffer(va);
		if (result != 0)
			return result;
		
		va->hrv->bib[va->hrv->in++] = va->hrv->line;
	}
	signals(0);
    g_signal_pid = 0;
	
	if (result < 0)
		return result;
		
	return heredoc_to_temp_file(va);
}

/**
 * Main handler for heredoc and input redirection
 */
int handle_red_if1(t_cmd *cmd, t_sp_var *va)
{
	int result = 0;
	
	(void) cmd;
	if (va->hrv->redir_type == REDIR_HEREDOC)
	{
		if (g_signal_pid != 5)
			result = process_heredoc(va);
		else
			return 10;
	}
	else if (va->hrv->redir_type == REDIR_IN && va->hrv->ss != 0)
		va->hrv->redir->fd = open(va->hrv->redir->file, O_RDONLY);
	return result;
}

int handle_red_if2(t_cmd *cmd, t_sp_var *va)
{
	if (va->hrv->redir->fd < 0 && va->hrv->ss != 0)
		return errno;
	if (!cmd->in)
		cmd->in = va->hrv->redir;
	else
	{
		va->hrv->current = cmd->in;
		while (va->hrv->current->next)
			va->hrv->current = va->hrv->current->next;
		va->hrv->current->next = va->hrv->redir;
	}
	return (0);
}

int open_outfile(t_sp_var *va)
{
	if (va->hrv->redir_type == REDIR_APPEND)
		va->hrv->redir->fd = open(va->hrv->redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		va->hrv->redir->fd = open(va->hrv->redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (va->hrv->redir->fd < 0)
		return errno;
	return (0);
}

void add_red_file(t_cmd *cmd, t_sp_var *va)
{
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