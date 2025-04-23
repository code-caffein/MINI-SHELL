/**
 * nneeww.c
 * Handles tokenization of input strings and token list management
 */

#include "he.h"

/**
 * Removes all instances of character c from string s
 * 
 * @param s Source string
 * @param c Character to remove
 * @return New string with c removed, or NULL on allocation failure
 */
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

/**
 * Checks if the latest token needs variable expansion
 * 
 * @param tokens Double pointer to token list
 * @param state Current quoting state
 */
void check_if_needs_expansion(t_token **tokens, t_quote_state state)
{
    t_token *current = *tokens;
    while (current && current->next)
        current = current->next;
        
    // Mark text tokens that contain $ for expansion, but only outside single quotes
    if (current->type == text && state != SINGLE_QUOTED)
    {
        int i = -1;
        while (current->value[++i])
        {
            if (current->value[i] == '$')
            {
                current->need_expand = true;
				printf("need_expand = true\n");
                break;
            }
        }
    }
}

/**
 * Checks if a string is a valid redirection or pipe token in the given quote state
 * 
 * @param buffer String to check
 * @param state Current quote state
 * @return 1 if valid operator, 0 if syntax error
 */
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

/**
 * Adds a token with an error flag to indicate syntax error
 * 
 * @param token Starting token in the list
 */
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
    new_token->need_expand = false;
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

/**
 * Creates a new token, determines its type, and adds it to the token list
 * 
 * @param tokens Double pointer to token list
 * @param buffer String content to tokenize
 * @param state Current quoting state
 * @para Last command exit status
 * @return 1 on success, 0 on failure (syntax error)
 */
int add_token_with_type(t_token **tokens, char *buffer, t_quote_state *state, bool wait_more_args)
{
	static char *tmp;
	static bool wait = false;
	// Check for empty buffer
	// Check for empty buffer
	if (buffer[0] == '\0')
		return 0;
	
    if (buffer[0] == '>' || buffer[0] == '<' || buffer[0] == '|')
    {
        if (check_operator_validity(buffer, state) == 0)
        {
            add_error_token(tokens);
            return 0;
        }
    }
    
	char *new_buff = NULL;
    // Remove quotes from the token
	if(*state == DOUBLE_QUOTED)
    	new_buff = remove_character(buffer, '\"');
	else if (*state == SINGLE_QUOTED)
		new_buff = remove_character(buffer, '\'');
	else if (*state == UNQUOTED)
		new_buff = ft_strdup(buffer);
    if (!new_buff)
        return 0;
    
    // Create the new token
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) {
        free(new_buff);
        return 0;
    }
    new_token->value = NULL;
    new_token->next = NULL;
    
    // Determine token type based on content
    if (ft_strcmp(new_buff, ">") == 0 && *state == UNQUOTED)
    {
        new_token->type = red;
        new_token->value = new_buff;
        new_token->heredoc = false;
    }
    else if (ft_strcmp(new_buff, "<") == 0 && *state == UNQUOTED)
    {
        new_token->type = red;
        new_token->value = new_buff;
        new_token->heredoc = false;
    }
    else if (ft_strcmp(new_buff, ">>") == 0 && *state == UNQUOTED)
    {
        new_token->type = red;
        new_token->value = new_buff;
        new_token->heredoc = false;        
    }
    else if (ft_strcmp(new_buff, "<<") == 0 && *state == UNQUOTED)
    {
        new_token->type = red;
        new_token->value = new_buff;
        new_token->heredoc = true;        
    }
    else if (ft_strcmp(new_buff, "|") == 0 && *state == UNQUOTED)
        new_token->type = pip;
    else
    {
        new_token->type = text;
        new_token->heredoc = false;
        new_token->value = new_buff;
    }
    // free(new_buff);
	expand_variables(new_token, state);
	
	if (wait_more_args)
	{
		// wait = true;
		tmp = ft_strjoin(new_token->value, tmp);
		free(new_token->value);
	}
	else
	{
		if (tmp)
		{
			new_token->value = ft_strjoin(tmp, new_token->value);
			free(tmp);
			tmp = NULL;
		}
		else
			new_token->value = ft_strdup(new_buff);
		if (*tokens == NULL)
		{
        	*tokens = new_token;
    	}
    	else
   		{
        	t_token *current = *tokens;
        	while (current->next)
        	    current = current->next;
        	current->next = new_token;
   		}
	}
    return 1;
}

/**
 * Identifies file tokens by looking for text tokens that follow redirection operators
 * 
 * @param tokens Pointer to token list
 */
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
                ft_strcmp(previous->value, "<") == 0)
            {
                current->type = file;
				while(current->next && current->next->type == text && current->next->wait_more_args == true)
				{
					current->next->type = file;
					current = current->next;
				}
            }
        }
        previous = current;
        current = current->next;
    }
}

/**
 * Checks if character is a token separator
 * 
 * @param c Character to check
 * @return true if character is whitespace or a redirection symbol
 */
int is_token_separator(char c)
{
    return (ft_isspace(c) || c == '>' || c == '<');
}

/**
 * Frees tokens from a starting point
 * 
 * @param start Pointer to first token to free
 */
void free_token_list(t_token *start)
{
    t_token *current;
    t_token *next;

    current = start;
    while (current)
    {
        next = current->next;
        if (current->value)
			free(current->value);
		if (current)
        	free(current);
        current = next;
    }
}

/**
 * Validates syntax for a token list
 * 
 * @param tokens Double pointer to token list
 * @return 1 if valid, 0 if invalid
 */
int validate_syntax(t_token **tokens)
{
    t_token *current = *tokens;
    t_token *previous;

    if (!current)
        return 1;
        
    previous = current;
    current = current->next;
    
    // Validate first token: can't start with pipe or redirection
    if (previous->type == pip || previous->type == red)
    {
        previous->syn_err = true;
        free_token_list(previous->next);
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
            free_token_list(previous->next);
            previous->next = NULL;
            return 0;
        }
        
        // Cannot have two redirections in a row
        if (current->type == red && previous->type == red)
        {
            previous->syn_err = true;
            free_token_list(previous->next);
            previous->next = NULL;
            return 0;
        }
        
        // Redirection must be followed by a filename (file or text)
        if (previous->type == red && current->type != file)
        {
            previous->syn_err = true;
            free_token_list(previous->next);
            previous->next = NULL;
            return 0;
        }
        
        // Heredoc requires a delimiter
        if (previous->type == red && ft_strcmp(previous->value, "<<") == 0)
        {
            if (!current || (current->type != file && current->type != text))
            {
                previous->syn_err = true;
                free_token_list(previous->next);
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
        free_token_list(previous->next);
        previous->next = NULL;
        return 0;
    }
    
    return 1;
}

/**
 * Tokenizes an input line into a linked list of command tokens
 * 
 * @param line Input command string to tokenize
 * @para Last command exit status
 * @return Linked list of tokens or NULL on error
 */
t_token *tokenize_input(char *line)
{
    t_token *tokens = NULL;
    char buffer[9999];
    int i = 0;
    int j = 0;
    t_quote_state state = UNQUOTED;
    bool has_heredoc = false;
	bool wait_more_args = false;

    // Process each character in the input line
    while (line[j])
    {
        char c = line[j];
        
        // Handle quote state transitions
        if (state == UNQUOTED && (c == '\'' || c == '"'))
        {
			if (i > 0 )
            {
				if (!is_token_separator(buffer[i - 1]))
					wait_more_args = true;
                buffer[i] = '\0';
                if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                {
                    free_token_list(tokens);
                    return NULL;
                }
				wait_more_args = false;
                i = 0;
            }
            if (c == '\'')
				state = SINGLE_QUOTED;
			else if (c == '"')
				state = DOUBLE_QUOTED;
            buffer[i++] = c;
        }
        // Handle end of quoted text with token separator after
        else if ((state == SINGLE_QUOTED && c == '\'' && (line[j + 1] != '\'' && line[j  + 1] != '"' && is_token_separator(line[j + 1]) || !line[j + 1])) || 
                (state == DOUBLE_QUOTED && c == '"'  && (line[j + 1] != '\'' && line[j  + 1] != '"' && is_token_separator(line[j + 1]) || !line[j + 1])))
        {
            buffer[i++] = c;
            buffer[i] = '\0';
            if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
            {
				i = 0;
				state = UNQUOTED;
				break;
			}
            state = UNQUOTED;
            i = 0;
        }
        // Handle end of quoted text with more content immediately after
        else if ((state == SINGLE_QUOTED && c == '\'' && line[j + 1] && (line[j + 1] == '\'' || line[j + 1] == '"' || !is_token_separator(line[j+1]))) || 
                (state == DOUBLE_QUOTED && c == '"' && line[j + 1] && (line[j + 1] == '\'' || line[j + 1] == '"' || !is_token_separator(line[j+1]))))
        {
            buffer[i++] = c;
            buffer[i] = '\0';
			wait_more_args = true;
            if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
            {
				i = 0;
				state = UNQUOTED;
				break;
			}
			wait_more_args = false;
            state = UNQUOTED;
            i = 0;
        }
        // Handle operators and special characters when unquoted
        else if (state == UNQUOTED && (ft_strchr("|<>", c) || buffer[0] == '|' || buffer[0] == '>' || buffer[0] == '<'))
        {
            // If we have content not starting with operators, tokenize it first
            if (i > 0 && buffer[0] != '<' && buffer[0] != '>' && buffer[0] != '|')
			{
				buffer[i] = '\0';
				if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
				{
					i = 0;
					break;
				}
				i = 0;
			}
            // Handle composite operators: << and >>
            else if (i > 0 && buffer[0] == '<' && c == '<')
            {
                buffer[i++] = c;
				if (!line[j + 1])
				{
					buffer[i] = '\0';
                	if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                	{
						i = 0;
						break;
					}
                	i = 0;
				}
            }
            else if (i > 0 && buffer[0] == '>' && c == '>')
            {
                buffer[i++] = c;
				if (!line[j + 1])
				{
					buffer[i] = '\0';
                	if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                	{
						i = 0;
						break;
					}
                	i = 0;
				}
            }
            // Handle < followed by non-<
            else if (i > 0 && buffer[0] == '<' && c != '<')
            {
                buffer[i] = '\0';
                if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                {
					// printf("\naaaaaaaaaaaaaaa\n");
					i = 0;
					break;
				}
                i = 0;
            }
            // Handle > followed by non->
            else if (i > 0 && buffer[0] == '>' && c != '>')
            {
                buffer[i] = '\0';
                if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                {
					i = 0;
					break;
				}
                i = 0;
            }
            // Handle pipe operator
            else if (i > 0 && buffer[0] == '|' && c == '|')
            {
                buffer[i++] = c;
				if (!line[j + 1])
				{
					buffer[i] = '\0';
                	if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                	{
						i = 0;
						break;
					}
                	i = 0;
				}
            }
            else if (i > 0 && buffer[0] == '|' && c != '|')
            {
                buffer[i] = '\0';
                if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                {
					i = 0;
					break;
				}
                i = 0;
            }
            // Start a new operator token
            else if (i == 0)
            {
                buffer[i++] = c;
            }
        }
        // Handle whitespace (token separator)
        else if (state == UNQUOTED && ft_isspace(c))
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
                {
					i = 0;
					break;
				}
                i = 0;
            }
        }
        // Handle regular characters
        else
            buffer[i++] = c;
		j++;
    }
	if (i > 0)
    {
        buffer[i] = '\0';
        if (!add_token_with_type(&tokens, buffer, &state, wait_more_args))
        {
            free_token_list(tokens);
            tokens = NULL;
        }
    }
    
    // Post-processing
    detect_file_tokens(&tokens);
    validate_syntax(&tokens);

    // Check for heredoc and syntax errors
    bool has_syntax_error = false;
    t_token *curr = tokens;
    t_token *prev = NULL;
    
    while(curr)
    {
        if (curr->heredoc == true)
            has_heredoc = true;
		// if (curr->syn_err == true)
		// 	printf("sgcsgcststcts:\n");
        prev = curr;
        curr = curr->next;
    }
    if (has_syntax_error == true)
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!:\n");
    // Return tokens or NULL based on errors and heredoc status
    if (prev && prev->syn_err && has_heredoc)
        return tokens;
    if (prev && prev->syn_err && !has_heredoc)
     {
       printf("Syntax error:\n");
	//    exit(1);
    }// Syntax error

    // Check for unclosed quotes
    if (i > 0 && state != UNQUOTED) 
	{
       printf("Syntax aaaaa error:\n");
	//    exit(1);
    }
    //open files !!!!
	//ft that open files
    if (prev && !prev->syn_err)
        return tokens;
    
    return NULL;
}


#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


int main(void)
{
    char *input;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
		
        if (*input != '\0')
            add_history(input);
		tokenize_input(input);
        free(input);
    }

    return 0;
}