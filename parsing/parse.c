
#include "he.h"

int g_signal_pid;





int handle_redirection(t_cmd *cmd, t_token *token, t_sp_var *va, int ss)
{
	int i = 0;
    if (!cmd || !token || !token->next)
        return 0;


    t_token *file_token = token->next;
    if (file_token->need_expand == true)
		i = 1;
    
    int redir_type = 0;
    
    if (ft_strcmp(token->value, "<") == 0)
        redir_type = REDIR_IN;
    else if (ft_strcmp(token->value, ">") == 0)
        redir_type = REDIR_OUT;
    else if (ft_strcmp(token->value, ">>") == 0)
        redir_type = REDIR_APPEND;
    else if (ft_strcmp(token->value, "<<") == 0)
        redir_type = REDIR_HEREDOC;
    t_redirection *redir = create_redirection(file_token->value, redir_type, va);
    t_redirection *current;

    if (redir_type == REDIR_IN || redir_type == REDIR_HEREDOC)
	{
		if (redir_type == REDIR_HEREDOC)
		{
			int in = 0;
			int capacity = 10;
			char **
			bib = mmallocc(sizeof(char *) * capacity, &va->allocs, P_GARBAGE); // Allocate initial memory
			if (!bib)
    			return errno;
			int n;
			char *line;
			while (1)
			{
				g_signal_pid = 2;
				if (g_signal_pid == -1)
					return -2;
    			write(1, "> ", 2);
   				n = heredoc_readline(&line, va);
    			if (!line || n == -2 || n == 0)
        			return -2;
				if (n > 0 && line[n - 1] == '\n')
    				line[n-1] = '\0';
    			if (ft_strcmp(line, file_token->value) == 0)
       				break;
    			if (i == 1)
				{
       				char *tmp;
        			t_quote_state state = UNQUOTED;
        			tmp = expand_env_vars(line, va);
        			line = tmp;
    			}
    			if (in >= capacity - 1)
				{
        			capacity *= 2;
        			char **new_bib = mmallocc(sizeof(char *) * capacity, &va->allocs, P_GARBAGE);
        			if (!new_bib)
            			return errno;
					int j = -1;
					while (++j < in)
						new_bib[j] = bib[j];
        			bib = new_bib;
    			}
    			bib[in++] = line;
			}
			g_signal_pid = 0;
			bib[in] = NULL;

			int fd = open("t_m_p_f_i_l_e", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   			if (fd < 0)
    		{
       			return errno;
			}
			int j = -1;
			while (++j < in)
			{
				write(fd, bib[j], ft_strlen(bib[j]));
				write(fd, "\n", 1);
			}
    		close(fd);
    		redir->fd = open("t_m_p_f_i_l_e", O_RDONLY);
		}
		else if (REDIR_IN && ss != 0)//input!!!
			redir->fd = open(redir->file, O_RDONLY);
		if (redir->fd < 0 && ss != 0)
		{
        	return errno;
    	}
        if (!cmd->in)
            cmd->in = redir;
		else
		{
			current = cmd->in;
            while (current->next)
                current = current->next;
            current->next = redir;
        }
    } else if ((redir_type == REDIR_OUT || redir_type == REDIR_APPEND) && ss != 0)
	{
		if (redir_type == REDIR_APPEND)
			redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->fd < 0)
		{
        	return errno;
    	}
        if (!cmd->out)
            cmd->out = redir;
        else
		{
			current = cmd->out;
            while (current->next)
                current = current->next;
            current->next = redir;
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
