/**
 * he.h - Header file for shell implementation
 * Contains structures and function prototypes for the shell
 */

#ifndef HE_H
#define HE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Token types enumeration
 */
typedef enum e_types {
    pip,    // Pipe operator (|)
    red,    // Redirection operator (<, >, >>, <<)
    file,   // File operand for redirection
    text    // Regular text/command/argument
} t_types;

/**
 * Quote state enumeration for parsing
 */
typedef enum e_quote_state {
    UNQUOTED,       // Not in quotes
    SINGLE_QUOTED,  // Inside single quotes
    DOUBLE_QUOTED   // Inside double quotes
} t_quote_state;

/**
 * Token structure for lexical analysis
 */
typedef struct s_token {
    char *value;            // Token content
    t_types type;           // Token type
    bool syn_err;           // Syntax error flag
    bool heredoc;           // Is heredoc delimiter
    bool need_expand;       // Needs variable expansion
    struct s_token *next;   // Next token in the list
} t_token;

/**
 * Redirection types
 */
#define REDIR_IN 1      // <  (input redirection)
#define REDIR_OUT 2     // >  (output redirection)
#define REDIR_APPEND 3  // >> (append output redirection)
#define REDIR_HEREDOC 4 // << (heredoc input)

/**
 * Redirection structure
 */
typedef struct s_redirection {
    char *file;                       // File name or heredoc delimiter
    int type;                         // Redirection type
    struct s_redirection *next;       // Next redirection in the list
} t_redirection;

/**
 * Command structure
 */
typedef struct s_cmd {
    char *name;                  // Command name
    char **args;                 // Arguments array
    bool *args_need_expand;      // Flags for which arguments need expansion
    int arg_count;               // Number of arguments
    int arg_capacity;            // Capacity of arguments array
    bool syn_err;                // Syntax error flag
    t_redirection *in;           // Input redirections list
    t_redirection *out;          // Output redirections list
    struct s_cmd *next;          // Next command (after pipe)
} t_cmd;

/*
 * String manipulation functions
 */
int     ft_strcmp(const char *s1, const char *s2);
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t  ft_strlen(const char *s);
char    **ft_split(char const *s, char c);
char    *ft_strdup(const char *s);
char    *ft_strchr(const char *s, int c);
int     ft_isspace(char c);

/*
 * Variable expansion functions
 */
int     expand_variables(t_token *tokens, t_quote_state *state, int exit_status);
char    *expand_env_vars(char *str, t_quote_state *state, int exit_status);
char    *get_var_value(char *var_name);

/*
 * Token and parsing functions
 */
t_token *tokenize_input(char *line, int exit_status);
void    detect_file_tokens(t_token **tokens);
int     validate_syntax(t_token **tokens);
t_cmd   *parse_tokens(t_token *tokens);

/*
 * Memory management functions
 */
void    free_token_list(t_token *start);
void    free_command(t_cmd *cmd);
void    free_commands(t_cmd *commands);

#endif /* HE_H */