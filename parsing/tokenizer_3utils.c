#include "he.h"

char *remove_character(char *s, char c)
{
    char *new_str;
    int i = -1;
    int j = 0;
	if (!s)
		return NULL;

    new_str = malloc(ft_strlen(s) + 1);
    if (!new_str)
        return NULL;
        
    while (s[++i])
    {
        if (s[i] != c)
            new_str[j++] = s[i];
    }
    new_str[j] = '\0';
    return new_str;
}

int check_operator_validity(char *buffer, t_quote_state *state)
{
    // Only treat as operators when unquoted 
    if (*state == UNQUOTED && buffer[0] == '>' && ft_strlen(buffer) <= 2)
        return 1;
    if (*state == UNQUOTED && buffer[0] == '<' && ft_strlen(buffer) <= 2)
        return 1;
    if (*state == UNQUOTED && buffer[0] == '|' && ft_strlen(buffer) == 1)
        return 1;
    return 0; // Not a valid operator in this context
}

void add_error_token(t_token **token)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;

    // Initialize all fields
    new_token->value = NULL;
    new_token->type = text;  // Default type instead of NULL
    new_token->syn_err = true;
    new_token->heredoc = false;
    // new_token->need_expand = false;
    new_token->wait_more_args = false;
    new_token->next = NULL;

    if (*token == NULL) {
        *token = new_token;
    } else {
        t_token *current = *token;  // Dereference here
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}

void detect_file_tokens(t_token **tokens)
{
    t_token *current = *tokens;
    t_token *previous = NULL;

    while (current != NULL)
    {
        // A file token is a text token that follows a redirection operator
        if (current->type == text && previous != NULL && previous->type == red)
        {
            if (ft_strcmp(previous->value, ">") == 0 || 
                ft_strcmp(previous->value, ">>") == 0 || 
                ft_strcmp(previous->value, "<") == 0 ||
				ft_strcmp(previous->value, "<<") == 0)
            {
                current->type = file;
				// while(current->next && current->next->type == text && current->next->wait_more_args == true)
				// {
				// 	current->next->type = file;
				// 	current = current->next;
				// }
            }
        }
        previous = current;
        current = current->next;
    }
}


int is_token_separator(char c)
{
    return (ft_isspace(c) || c == '>' || c == '<' || c == '|');
}

/**
 * Frees tokens from a starting point
 * 
 * @param start Pointer to first token to free
 */
void free_token_list(t_token **start)
{
    t_token *current;
    t_token *next;

	if (!start || !*start)
		return;
    current = *start;
    while (current)
    {
        next = current->next;
        if (current->value)
			free(current->value);
		if (current)
        	free(current);
        current = next;
    }
	*start = NULL;
}

int validate_syntax(t_token **tokens)
{
    t_token *current = *tokens;
    t_token *previous;

    if (!current || !*tokens)
        return 1;
        
    previous = current;
    current = current->next;
    
    // Validate first token: can't start with pipe or redirection
    if (previous->type == pip || previous->type == red)
    {
		// printf("llllllllllllllllll\n");
        previous->syn_err = true;
        free_token_list(&previous->next);
        previous->next = NULL;
        return 0;
    }
    
    // Validate token pairs throughout the list
    while (current)
    {
        // Cannot have pipe after pipe or redirection
        if (current->type == pip && (previous->type == pip || previous->type == red))
        {
            previous->syn_err = true;
            free_token_list(&current);
            previous->next = NULL;
            return 0;
        }
        
        // Cannot have two redirections in a row
        if (current->type == red && previous->type == red)
        {
			// printf("sgcsgcststcts:\n");
            previous->syn_err = true;
            free_token_list(&current);
            previous->next = NULL;
            return 0;
        }
        
        // Redirection must be followed by a filename (file or text)
        if (previous->type == red && current->type != file)
        {
            previous->syn_err = true;
            free_token_list(&current);
            previous->next = NULL;
            return 0;
        }
        
        // Heredoc requires a delimiter
        if (previous->type == red && ft_strcmp(previous->value, "<<") == 0)
        {
            if (!current || (current->type != file && current->type != text))
            {
                previous->syn_err = true;
                free_token_list(&current);
                previous->next = NULL;
                return 0;
            }
        }
        
        previous = current;
        current = current->next;
    }
    
    // Validate final token: can't end with pipe or redirection
    if (previous->type == pip || previous->type == red || previous->syn_err)
    {
        previous->syn_err = true;
        free_token_list(&previous->next);
        previous->next = NULL;
        return 0;
    }
    
    return 1;
}