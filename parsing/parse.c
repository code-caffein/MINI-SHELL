
#include "he.h"
#include <sys/ioctl.h>
int g_signal_pid;






t_cmd *create_new_command(t_sp_var *va)
{
    t_cmd *cmd = mmallocc(sizeof(t_cmd), &va->allocs, P_GARBAGE);
    if (!cmd)
        return NULL;
    
    cmd->name = NULL;
    cmd->arg_count = 0;
    cmd->arg_capacity = 10;
    cmd->args = mmallocc(sizeof(char *) * (cmd->arg_capacity + 1), &va->allocs, P_GARBAGE);
    
    if (!cmd->args) {
        free(cmd);
        return NULL;
    }

	int i = -1;
    while (++i <= cmd->arg_capacity)
        cmd->args[i] = NULL;
    
	// cmd->syn_err = false;
    cmd->in = NULL;
    cmd->out = NULL;
    cmd->next = NULL;
    return cmd;
}


void add_argument(t_cmd *cmd, char *arg, t_sp_var *va)
{
    if (!cmd || !arg)
        return;

    if (cmd->arg_count == 0)
        cmd->name = ft_strdup(arg, &va->allocs, P_GARBAGE);
	if (!cmd->name)
		return;
    if (cmd->arg_count >= cmd->arg_capacity)
	{
        cmd->arg_capacity *= 2;
        char **new_args = mmallocc(sizeof(char *) * (cmd->arg_capacity + 1), &va->allocs, P_GARBAGE);
        if (!new_args)
            return;
		int i = -1;
        while (++i < cmd->arg_count)
            new_args[i] = cmd->args[i];
		i = cmd->arg_count - 1;
        while (++i <= cmd->arg_capacity)
            new_args[i] = NULL;
        free(cmd->args);
        cmd->args = new_args;
    }
    cmd->args[cmd->arg_count] = ft_strdup(arg, &va->allocs, P_GARBAGE);
    cmd->arg_count++;
    cmd->args[cmd->arg_count] = NULL;
}

int heredoc_readline(char **out, t_sp_var *va)
{
    int cap = 128, len = 0;
    char *buf = mmallocc(cap, &va->allocs, P_GARBAGE);
    if (!buf) return -1;
	
    while (1)
    {
		char c;
		int r = read(0, &c, 1);   
		if (r == 0)     // EOF
			return 0;
        if (r < 0)
    {
        if (errno == EINTR) {
                if (g_signal_pid == -1) {
                    // Flush pending input using ioctl and read
                    int bytes_available;
                    while (1) {
                        if (ioctl(0, FIONREAD, &bytes_available) == -1 || bytes_available <= 0)
                            break;
                        char dummy[256];
                        if (read(0, dummy, sizeof(dummy)) <= 0)
                            break;
                    }
                }
					rl_replace_line("", 0);
            		rl_redisplay();
                    // free(buf);
                    *out = NULL;
                    return -2;
            }
        // free(buf);
        return -1;
    }

        // append c
        if (len + 1 >= cap)
        {
            cap *= 2;
            char *t = realloc(buf, cap);
            if (!t) { 
				// free(buf);
				return -1; 
			}
            buf = t;
        }
        buf[len++] = c;
        if (c == '\n')   break;
    }
	

    buf[len] = '\0';
    *out = buf;
	// printf("[%s]\n",*out);
    return (ssize_t)len;
}

t_redirection	*create_redirection(char *file, int type, t_sp_var *va)
{
	t_redirection	*redir;

	redir = mmallocc(sizeof(t_redirection), &va->allocs, P_GARBAGE);
	if (!redir)
		return NULL;
	redir->file = ft_strdup(file, &va->allocs, P_GARBAGE);
	redir->type = type;
	redir->fd = -1;
	redir->next = NULL;
	// redir->err_type = 0;
	return (redir);
}


int handle_redirection(t_cmd *cmd, t_token *token, t_sp_var *va, int ss)
{
	// static int a = 0;
	int i = 0;
    if (!cmd || !token || !token->next)
        return 0;
    // int saved_stdin = dup(STDIN_FILENO);
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
			{
    			// perror("malloc");
    			// free(redir->file);
    			// free(redir);
    			return errno;
			}
			ssize_t n;
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
				if (n > 0 && line[n-1] == '\n')
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
		else//output!!!
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
	t_cmd *commands = NULL;
	t_cmd *current_cmd = NULL;
	t_token *current = tokens;
	t_token *prev = NULL;
	int s = -1;
	int tmp_err = 0;
	int err = 0;
	bool syn_err;
	char *err_file;

	syn_err = false;
	while(current)
	{
		if(current->syn_err)
			syn_err = true;
		
		prev = current;
		current = current->next;
	}
	current = tokens;
	commands = create_new_command(va);
	if (!commands)
		return NULL;
	
	current_cmd = commands;
	while (current && !current->syn_err)
	{
		// printf("[%d]\n",current->type);
		if (current->type == pip)
		{
			if (err != 0 && tmp_err != -2)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(err_file, 2);
				write(2,":",1);
				ft_putstr_fd(strerror(err), 2);
				write(2,"\n",1);
				
			}
			current_cmd->next = create_new_command(va);
			if (!current_cmd->next)
				return NULL;
			s = 1;
			current_cmd = current_cmd->next;
		}
		else if (current->type == red)
		{
            if (!current->next || current->next->type != file)
			{
        		printf("Syntax error: Missing filename after redirection\n");
        		// free_commands(commands);
        		return NULL;
			}
			else if (current->next && current->next->type == file)
			{
				tmp_err = handle_redirection(current_cmd, current, va, s);
				if (err == 0)
				{
					err = tmp_err;
					err_file = current->next->value;
				}
				if (tmp_err == -2)//heredoc ctrl c return 
				{
    				if (current_cmd && current_cmd->name)
					{
						current_cmd->name = NULL;
						return commands;
					}	
					
				}
				if (tmp_err != 0)
				{
					s = 0;
					if (current_cmd && current_cmd->name)
						current_cmd->name = NULL;
				}
				current = current->next; // skipp file token!!1
       		}
        }
        else if (current->type == text)
			add_argument(current_cmd, current->value, va);
        if (current)
            current = current->next;
    }
	
	if (err != 0 && tmp_err != -2)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(err_file, 2);
		write(2,":",1);
		ft_putstr_fd(strerror(err), 2);
		write(2,"\n",1);
				
	}

	if (syn_err)
		return (NULL);
    return commands;
}
