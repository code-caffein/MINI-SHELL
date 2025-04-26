#include "he.h"

// char *remove_character(char *s, char c)
// {
//     char *new_str;
//     int i = -1;
//     int j = 0;
// 	if (!s)
// 		return NULL;

//     new_str = malloc(ft_strlen(s) + 1);
//     if (!new_str)
//         return NULL;
        
//     while (s[++i])
//     {
//         if (s[i] != c)
//             new_str[j++] = s[i];
//     }
//     new_str[j] = '\0';
//     return new_str;
// }

// int check_operator_validity(char *buffer, t_quote_state *state)
// {
//     // Only treat as operators when unquoted 
//     if (*state == UNQUOTED && buffer[0] == '>' && ft_strlen(buffer) <= 2)
//         return 1;
//     if (*state == UNQUOTED && buffer[0] == '<' && ft_strlen(buffer) <= 2)
//         return 1;
//     if (*state == UNQUOTED && buffer[0] == '|' && ft_strlen(buffer) == 1)
//         return 1;
//     return 0; // Not a valid operator in this context
// }

// void add_error_token(t_token **token)
// {
//     t_token *new_token = malloc(sizeof(t_token));
//     if (!new_token)
//         return;

//     // Initialize all fields
//     new_token->value = NULL;
//     new_token->type = text;  // Default type instead of NULL
//     new_token->syn_err = true;
//     new_token->heredoc = false;
//     // new_token->need_expand = false;
//     new_token->wait_more_args = false;
//     new_token->next = NULL;

//     if (*token == NULL) {
//         *token = new_token;
//     } else {
//         t_token *current = *token;  // Dereference here
//         while (current->next)
//             current = current->next;
//         current->next = new_token;
//     }
// }

