/**
 * parse.c
 * Contains functions for parsing tokens into command structures
 */

#include "he.h"

/**
 * Creates a new command structure with default values
 * 
 * @return Newly allocated command structure or NULL on failure
 */
t_cmd *create_new_command(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;
    
    // Initialize command fields
    cmd->arg_count = 0;
    cmd->arg_capacity = 10;
	cmd->name = NULL;


    if (!cmd->args)
    {
        free(cmd);
        return NULL;
    }

	    // Initialize arguments array to NULL
    int i = -1;
    while (++i <= cmd->arg_capacity)
        cmd->args[i] = NULL;
    
    cmd->syn_err = false;
    cmd->next = NULL;
    return cmd;
}

/**
 * Adds an argument to a command
 * 
 * @param cmd Command to add argument to
 * @param arg Argument string
 * @param arg_need_expand Flag if argument needs variable expansion
 */
void add_argument(t_cmd *cmd, t_token *token, int redir_type) 
{
	t_token *curr = token;
    if (!cmd || !curr->value)
        return;

    // First argument becomes command name
	if(cmd->arg_count == 0)
	{
		cmd->name = ft_strdup(curr->value);
		if (!cmd->name)
			return;
	}


    // Resize argument array if needed
    if (cmd->arg_count >= cmd->arg_capacity)
    {
        cmd->arg_capacity *= 2;
        char **new_args = malloc(sizeof(t_arg *) * (cmd->arg_capacity + 1));
        
        if (!new_args)
            return;
        
        // Copy existing arguments
        int i = -1;
        while (++i < cmd->arg_count)
            new_args[i] = cmd->args[i];
        
        // Initialize new slots to NULL
        i = cmd->arg_count - 1;
        while (++i <= cmd->arg_capacity)
            new_args[i] = NULL;
        
        free(cmd->args);
        cmd->args = new_args;
    }
    
    // Add the new argum
	cmd->args[cmd->arg_count] = malloc(sizeof(t_arg));
	if (!cmd->args[cmd->arg_count])
		return;
    cmd->args[cmd->arg_count]->need_expand = curr->need_expand;
	cmd->args[cmd->arg_count]->wait_more_args = curr->wait_more_args;
	if (!redir_type)
		cmd->args[cmd->arg_count]->type = text;
	else
	{
		if (redir_type == REDIR_IN)
			cmd->args[cmd->arg_count]->type = input;
		else if (redir_type == REDIR_OUT)
			cmd->args[cmd->arg_count]->type = output;
		else if (redir_type == REDIR_APPEND)
			cmd->args[cmd->arg_count]->type = append;
		else if (redir_type == REDIR_HEREDOC)
			cmd->args[cmd->arg_count]->type = heredoc;
	}
	cmd->args[cmd->arg_count]->value = ft_strdup(curr->value);
    cmd->arg_count++;
    cmd->args[cmd->arg_count] = NULL;
}

/**
 * Creates a new redirection structure
 * 
 * @param file File or delimiter name
 * @param type Redirection type (REDIR_IN, REDIR_OUT, etc.)
 * @return New redirection structure or NULL on failure
 */
t_redirection *create_redirection(char *file, int type)
{
    t_redirection *redir;

    redir = malloc(sizeof(t_redirection));
    if (!redir)
        return NULL;
        
    redir->file = ft_strdup(file);
    redir->type = type;
    redir->next = NULL;
    return redir;
}

/**
 * Process a redirection token and adds it to the command structure
 * 
 * @param cmd Command to add redirection to
 * @param token Redirection token
 */
int get_redirection(t_token *token)
{
    if (!token)
        return;
    
    int redir_type = 0;
    
    // Determine redirection type
    if (ft_strcmp(token->value, "<") == 0)
        redir_type = REDIR_IN;
    else if (ft_strcmp(token->value, ">") == 0)
        redir_type = REDIR_OUT;
    else if (ft_strcmp(token->value, ">>") == 0)
        redir_type = REDIR_APPEND;
    else if (ft_strcmp(token->value, "<<") == 0)
        redir_type = REDIR_HEREDOC;
    
	return redir_type;
}

/**
 * Frees a single command structure and all its resources
 * 
 * @param cmd Command to free
 */
void free_command(t_cmd *cmd)
{
    if (!cmd)
        return;
    
    // Free command name
    if (cmd->name)
        free(cmd->name);
    
    // Free argument list and the argument structures
    if (cmd->args)
    {
        int i = 0;
        while (i < cmd->arg_count)
        {
            if (cmd->args[i])
            {
                if (cmd->args[i]->value)
                    free(cmd->args[i]->value);
                free(cmd->args[i]);
            }
            i++;
        }
        free(cmd->args);
    }
    
    free(cmd);
}

/**
 * Frees a linked list of commands
 * 
 * @param commands First command in the list
 */
void free_commands(t_cmd *cmd)
{
    t_cmd *current;
    t_cmd *next;
    
    current = cmd;
    while (current)
    {
        next = current->next;
        free_command(current);
        current = next;
    }
}

/**
 * Parses a token list into a command list
 * 
 * @param tokens Token list to parse
 * @return Linked list of command structures or NULL on error
 */
t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *commands = NULL;
    t_cmd *current_cmd = NULL;
    t_token *current = tokens;

    // Create first command
    commands = create_new_command();
    if (!commands)
    {
        free_commands(commands);
        return NULL;
    }
    
    current_cmd = commands;
    
    // Process all tokens
	int red = 0;
    while (current && !current->syn_err)
    {
        if (current->type == pip)
        {
            // Create a new command for each pipe
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
            // Handle redirections
            if (!current->next || current->next->type != file)
            {
                printf("Syntax error: Missing filename after redirection\n");
                free_commands(commands);
                return NULL;
            }
            else if (current->next && current->next->type == file)
                red = get_redirection(current);
        }
        else if (current->type == text || current->type == file)
        {
            // Add arguments
            add_argument(current_cmd, current, red);
        }
        
        if (current)
            current = current->next;
    }
    
    // Handle syntax error in token list
    if (current && current->syn_err)
        current_cmd->syn_err = true;
        
    return commands;
}