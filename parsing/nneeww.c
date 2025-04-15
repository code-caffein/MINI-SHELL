#include "he.h"

char *remove_c(char *s, char c)
{
	char	*new;
	int i =-1;
	int j = 0;

	new = malloc(ft_strlen(s) + 1);
	if(!new)
		return (NULL);
	while (s[++i])
	{
		if (s[i] != c)
			new[j++] = s[i];
	}
	new[j] = '\0';
	return (new);
}
int pr_check(char *buffer, t_quote_state *state)
{
	if (state == UNQUOTED && (ft_strcmp(buffer, ">") == 0 || ft_strcmp(buffer, ">>") == 0))
		return(1);
	if (state == UNQUOTED && (ft_strcmp(buffer, "<") == 0 || ft_strcmp(buffer, "<<") == 0))
		return(1);
	if (state == UNQUOTED && (ft_strcmp(buffer, "|") == 0))
		return(1);
	return (0);// write syntax error !!!!!!
}

void add_err_token(t_token *token)
{
	t_token *new_token = malloc(sizeof(t_token));
	if (!new_token)
        return (NULL);
	if (token == NULL) {
        token = new_token;
    }
	else
	{
        t_token *current = token;
        while (current->next)
            current = current->next;
        current->next = new_token;
		new_token->syn_err = true;
		new_token->next = NULL;
    }
}

int get_type_add_token(t_token **tokens, char *buffer, t_quote_state *state)
{

	if (buffer[0] == '>' || buffer[0] == '<' || buffer[0] == '|')
	{
		if (!pr_check(buffer, state))
		{
			add_err_token(*tokens);
			return 0;
		}
	}
	char *tmp = remove_c(buffer, '\"');
    if (!tmp)
		return ;
    
    char *new_buff = remove_c(tmp, '\'');
	if(!new_buff)
		return ;
    free(tmp);
    
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) {
        free(new_buff);
        return;
    }
    
    new_token->value = ft_strdup(new_buff);
    new_token->next = NULL;
	if (ft_strcmp(new_buff, "echo") == 0)
	{
		new_token->type = cmd;
		new_token->value = "echo";
		new_token->heredoc = false;
	}
    else if (ft_strcmp(new_buff, "-n") == 0)
	{
		new_token->type = cmd;
		new_token->value = "-n";
		new_token->heredoc = false;		
	}
    else if (ft_strcmp(new_buff, "cd") == 0)
	{
		new_token->type = cmd;
		new_token->value = "cd";
		new_token->heredoc = false;		
	}
    else if (ft_strcmp(new_buff, "pwd") == 0)
	{
		new_token->type = cmd;
		new_token->value = "pwd";
		new_token->heredoc = false;	
	}
    else if (ft_strcmp(new_buff, "export") == 0)
	{
		new_token->type = cmd;
		new_token->value = "export";
		new_token->heredoc = false;		
	}
    else if (ft_strcmp(new_buff, "unset") == 0)
	{
		new_token->type = cmd;
		new_token->value = "unset";
		new_token->heredoc = false;		
	}
    else if (ft_strcmp(new_buff, "env") == 0)
	{
		new_token->type = cmd;
		new_token->value = "env";
		new_token->heredoc = false;		
	}
    else if (ft_strcmp(new_buff, "exit") == 0)
	{
		new_token->type = cmd;
		new_token->value = "exit";
		new_token->heredoc = false;		
	}
    else if (ft_strcmp(new_buff, ">") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = ">";
		new_token->heredoc = false;
	}
    else if (ft_strcmp(new_buff, "<") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = "<";
		new_token->heredoc = false;
	}
    else if (ft_strcmp(new_buff, ">>") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = ">>";
		new_token->heredoc = false;		
	}
    else if (ft_strcmp(new_buff, "<<") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = "<<";
		new_token->heredoc = true;		
	}
    else if (ft_strcmp(new_buff, "|") == 0 && state == UNQUOTED)
		new_token->type = pip;
    else
	{
		new_token->type = text;
		new_token->heredoc = false;
	}
	free(new_buff);

    if (*tokens == NULL) {
        *tokens = new_token;
    }
	else
	{
        t_token *current = *tokens;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
	detect_file(tokens);
	expand_variables(tokens, state);
	return (1);
}


void detect_file(t_token *tokens)
{
    t_token *current = tokens;
    t_token *previous = NULL;

    while (current != NULL)
    {
        if (current->type == text && previous != NULL && previous->type == red)
        {
            if (ft_strcmp(previous->value, ">") == 0 || 
                ft_strcmp(previous->value, ">>") == 0 || 
                ft_strcmp(previous->value, "<") == 0)
            {
                current->type = file;
            }
        }
        previous = current;
        current = current->next;
    }
}
int ft_stop(char c)
{
	return (ft_isspace(c) || c == '>' || c == '<');
}

void free_remaining_token(t_token *start)
{
	t_token	*current;
	t_token	*next;

	current = start;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}


int syntax_valid(t_token **tokens) //// check!!!!!!!!!!!!!!!!!!
{
	t_token *current = *tokens;
	t_token *previous;

	if (!current)
        return 1;
	previous = current;
	current = current->next;
	if (previous->type == pip || previous->type == red)
	{
		previous->syn_err = true;
		free_remaining_token(previous->next);
		previous->next = NULL;
		return (0);
	}
	while (current)
	{
		if (current->type == pip && (previous->type == pip || previous->type == red))
		{
			previous->syn_err = true;
			free_remaining_token(previous->next);
			previous->next = NULL;
			return (0);
		}
		if (current->type == red && previous->type == red)
		{
			previous->syn_err = true;
			free_remaining_token(previous->next);
			previous->next = NULL;
			return (0);
		}
		if (previous->type == red && current->type != file && current->type != text)
		{
			previous->syn_err = true;
			free_remaining_token(previous->next);
			previous->next = NULL;
			return (0);
		}
		if (previous->type == red && ft_strcmp(previous->value, "<<") == 0)
		{
			if (!current || (current->type != file && current->type != text))
			{
				previous->syn_err = true;
				free_remaining_token(previous->next);
				previous->next = NULL;
				return (0);
			}
		}
		previous = current;
		current = current->next;
	}
	if (previous->type == pip || previous->type == red)
	{
		previous->syn_err = true;
		free_remaining_token(previous->next);
		previous->next = NULL;
		return (0);
	}
	return (1);
}

t_token	*sep(char *line)
{
    t_token *tokens = NULL;
    char buffer[9999];
    int i = 0;
	int j = -1;
    t_quote_state state = UNQUOTED;


    while (line[++j])
	{
        char c = line[j];
        if (state == UNQUOTED && (c == '\'' || c == '"'))
		{
			if (c == '\'')				state = SINGLE_QUOTED;
			else
				state = DOUBLE_QUOTED;
            buffer[i++] = c;
        }
		else if ((state == SINGLE_QUOTED && c == '\'' && line[j + 1] && line[j + 1] != '\'' && line[j + 1] != '"' && ft_stop(line[j+1])) || 
				(state == DOUBLE_QUOTED && c == '"' && line[j + 1] && line[j + 1] != '\'' && line[j + 1] != '"' && ft_stop(line[j+1])))
		{
            buffer[i++] = c;
            buffer[i] = '\0';
            if (!get_type_add_token(&tokens, buffer, state));
				break ;
            state = UNQUOTED;
            i = 0;
        }
		else if ((state == SINGLE_QUOTED && c == '\'' && line[j + 1] && (line[j + 1] == '\'' || line[j + 1] == '"' || !ft_stop(line[j+1]))) || 
				(state == DOUBLE_QUOTED && c == '"' && line[j + 1] && (line[j + 1] == '\'' || line[j + 1] == '"' || !ft_stop(line[j+1]))))
		{
			buffer[i++] = c;
			state = UNQUOTED;
		}
		else if (state == UNQUOTED && (ft_strchr("|<>", c) || buffer[0] == '|' || buffer[0] == '>' || buffer[0] == '<'))
		{

            if (i > 0 && buffer[0] != '<' && buffer[0] != '>')
			{
                buffer[i] = '\0';
                if (!get_type_add_token(&tokens, buffer, state));
					break ;
                i = 0;
            }
			else if (i > 0 && buffer[0] == '<' && c == '<')
			{
				buffer[i++] = c;
			}
			else if (i > 0 && buffer[0] == '>' && c == '>')
			{
				buffer[i++] = c;
			}
			else if (i > 0 && buffer[0] == '<' && c != '<')
			{
				buffer[i] = '\0';
				if (!get_type_add_token(&tokens, buffer, state));
					break ;
				i = 0;
			}
			else if (i > 0 && buffer[0] == '>' && c != '>')
			{
				buffer[i] = '\0';
				if (!get_type_add_token(&tokens, buffer, state));
					break ;
				i = 0;
			}
			else if (i > 0 && buffer[0] == '|' && c == '|')
			{
				buffer[i++] = c;
			}
			else if (i > 0 && buffer[0] == '|' && c != '|')
			{
				buffer[i++] = c;
				buffer[i] = '\0';
				if (!get_type_add_token(&tokens, buffer, state));
					break ;
				i = 0;
			}
			else if (i == 0)
			{
				buffer[i++] = c;
			}
        }
		else if (state == UNQUOTED && ft_isspace(c))
		{
            if (i > 0)
			{
                buffer[i] = '\0';
                if (!get_type_add_token(&tokens, buffer, state));
					break ;
                i = 0;
            }
        }
		else
            buffer[i++] = c;
    }

	syntax_valid(tokens);

	bool here_doc;
	t_token *curr = tokens;
	t_token *prev;
	while(curr)
	{
		if (curr->heredoc == true)
			here_doc = true;
		prev = curr;
		curr = curr->next;
	}
	if (prev->syn_err && here_doc)
		return (tokens);
	if (prev->syn_err && !here_doc)
		return (NULL);//!!!!!!!!!!!!!!! SYNTAX ERR !!!!



    if (i > 0 && state != UNQUOTED) {
        
		//exit erroor!!!!!!!!!!!!free
	}
	if (!prev->syn_err)
		return tokens;
}
