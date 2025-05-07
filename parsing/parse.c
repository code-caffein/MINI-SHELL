
#include "he.h"








t_cmd *create_new_command(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;
    
    cmd->name = NULL;
    cmd->arg_count = 0;
    cmd->arg_capacity = 10;
    cmd->args = malloc(sizeof(char *) * (cmd->arg_capacity + 1));
    
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


void add_argument(t_cmd *cmd, char *arg)
{
    if (!cmd || !arg)
        return;

    if (cmd->arg_count == 0)
        cmd->name = ft_strdup(arg);
	if (!cmd->name)
	{
		perror("malloc");
		return;
	}
	// printf("name: %s\n", cmd->name);
	// printf("arg_count: %d\n", cmd->arg_count);
	// printf("arg_capacity: %d\n", cmd->arg_capacity);
	//
	// printf("arg: %s\n", arg);

    
    if (cmd->arg_count >= cmd->arg_capacity)
	{
        cmd->arg_capacity *= 2;
        char **new_args = malloc(sizeof(char *) * (cmd->arg_capacity + 1));
        
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
    
    cmd->args[cmd->arg_count] = ft_strdup(arg);
    cmd->arg_count++;
    cmd->args[cmd->arg_count] = NULL;
}


t_redirection	*create_redirection(char *file, int type)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return NULL;
	redir->file = ft_strdup(file);
	redir->type = type;
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}


void handle_redirection(t_cmd *cmd, t_token *token, t_env *env, int status)
{
	int i = 0;
    if (!cmd || !token || !token->next)
        return;
    
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
    
    t_redirection *redir = create_redirection(file_token->value, redir_type);
    t_redirection *current;

    if (redir_type == REDIR_IN || redir_type == REDIR_HEREDOC)
	{
		if (redir_type == REDIR_HEREDOC)
		{
    		// int fd = open("t_m_p_f_i_l_e", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   			// if (fd < 0)
    		// {
       		// 	perror("open");
        	// 	free(redir->file);
        	// 	free(redir);
        	// 	return;
			// }
			int in = 0;
			int capacity = 10;
			char **bib = malloc(sizeof(char *) * capacity); // Allocate initial memory
			if (!bib)
			{
    			perror("malloc");
    			free(redir->file);
    			free(redir);
    			return;
			}

			char *line;
			while (1)
			{
    			line = readline("> ");
    			if (!line)
        			break;
    			if (ft_strcmp(line, file_token->value) == 0)
				{
        			free(line);
       				break;
    			}
    			if (i == 1)
				{
       				char *tmp;
        			t_quote_state state = UNQUOTED;
        			tmp = expand_env_vars(line, &state, env, status);
        			free(line);
        			line = tmp;
    			}
    // Check if we need to resize bib
    			if (in >= capacity - 1)
				{
        			capacity *= 2;
        			char **new_bib = malloc(sizeof(char *) * capacity);
        			if (!new_bib)
					{
            			perror("realloc");
            			break;
        			}
					int j = -1;
					while (++j < in)
						new_bib[j] = bib[j];
					free(bib);
        			bib = new_bib;
    			}
    			bib[in++] = line;
			}
			bib[in] = NULL; // Null-terminate the array

			int fd = open("t_m_p_f_i_l_e", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   			if (fd < 0)
    		{
       			perror("open");
        		free(redir->file);
        		free(redir);
        		return;
			}
			int j = -1;
			while (++j < in)
			{
				write(fd, bib[j], ft_strlen(bib[j]));
				write(fd, "\n", 1);
				free(bib[j]);
			}
			free (bib);
    		close(fd);
    
    		redir->fd = open("t_m_p_f_i_l_e", O_RDONLY | O_CREAT);
    		if (redir->fd < 0)
    		{
        		perror("open heredoc tmp");
        		free(redir->file);
        		free(redir);
        		return;
    		}
		}
		else//input!!!
			redir->fd = open(redir->file, O_RDONLY);
		if (redir->fd < 0)
		    perror("open");
        if (!cmd->in)
            cmd->in = redir;
		else
		{
			current = cmd->in;
            while (current->next)
                current = current->next;
            current->next = redir;
        }
    } else if (redir_type == REDIR_OUT || redir_type == REDIR_APPEND)
	{
		if (redir_type == REDIR_APPEND)
			redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else//output!!!
			redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->fd < 0)
		    perror("open");
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
}

void free_command(t_cmd *cmd)
{
    if (!cmd)
        return;
    
    if (cmd->name)
        free(cmd->name);
    
    if (cmd->args)
	{
		int i = -1;
        while ( ++i < cmd->arg_count)
		{
            if (cmd->args[i])
                free(cmd->args[i]);
        }
        free(cmd->args);
    }
    
    t_redirection *in_current = cmd->in;
    while (in_current) {
        t_redirection *temp = in_current;
        in_current = in_current->next;
        if (temp->file)
            free(temp->file);
        free(temp);
    }
    
    t_redirection *out_current = cmd->out;
    while (out_current) {
        t_redirection *temp = out_current;
        out_current = out_current->next;
        if (temp->file)
            free(temp->file);
        free(temp);
    }
    
    free(cmd);
}

void	free_commands(t_cmd *commands)
{
	t_cmd *current;
	t_cmd *temp;

	current = commands;
	while (current)
	{
		temp = current;
		current = current->next;
		free_command(temp);
	}
}


















t_cmd *parse_tokens(t_token *tokens, t_env *env, int status)
{
	t_cmd *commands = NULL;
	t_cmd *current_cmd = NULL;
	t_token *current = tokens;
	t_token *prev = NULL;
	bool syn_err;

	syn_err = false;
	while(current)
	{
		if(current->syn_err)
			syn_err = true;
		
		prev = current;
		current = current->next;
	}
	// if (syn_err)
	// 	printf("syntax error near unexpected token \"%s\"\n", prev->value);
	current = tokens;
	commands = create_new_command();
	if (!commands)
	{
		free_commands(commands);
		return NULL;
	}
	
	current_cmd = commands;
	while (current && !current->syn_err)
	{
		if (current->type == pip)
		{
			current_cmd->next = create_new_command();
			if (!current_cmd->next)
			{
				free_commands(commands);
				return NULL;
			}
			current_cmd = current_cmd->next;
		}
		else if (current->type == red)
		{
            if (!current->next || current->next->type != file)
			{
        		printf("Syntax error: Missing filename after redirection\n");
        		free_commands(commands);
        		return NULL;
			}
			else if (current->next && current->next->type == file)
			{
				handle_redirection(current_cmd, current, env, status);
				current = current->next; // skipp file token!!1
       		}
        }
        else if (current->type == text)
        {
			add_argument(current_cmd, current->value);
			// printf("arg: %s\n", current->value);
		}
        if (current)
            current = current->next;
    }
	if (syn_err)
	{
		//free cmds
		return (NULL);
	}
	// if (current && current->syn_err)
	// 	current_cmd->syn_err = true;
    return commands;
}
