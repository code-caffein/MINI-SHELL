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
    
    if (!cmd->args)
	{
        free(cmd);
        return NULL;
    }

	int i = -1;
    while (++i <= cmd->arg_capacity)
        cmd->args[i] = NULL;
    
	cmd->syn_err = false;
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
	redir->next = NULL;
	return (redir);
}


void handle_redirection(t_cmd *cmd, t_token *token)
{
    if (!cmd || !token || !token->next)
        return;
    
    t_token *file_token = token->next;
    
    
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











t_cmd *parse_tokens(t_token *tokens)
{
	t_cmd *commands = NULL;
	t_cmd *current_cmd = NULL;
	t_token *current = tokens;

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
			handle_redirection(current_cmd, current);
			current = current->next;
		}
		else if (current->type == cmd || current->type == text || current->type == file)
			add_argument(current_cmd, current->value);
		if (current)
			current = current->next;
	}
	if (current && current->syn_err)
		current_cmd->syn_err = true;
	return commands;
}
// 


//////