#ifndef HE_H
#define HE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <signal.h>

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
    // bool need_expand;       // Needs variable expansion
	bool wait_more_args; // Wait for more arguments
    struct s_token *next;   // Next token in the list
} t_token;

typedef struct s_var
{
	t_token *tokens;
    char *buffer;
    int i;
    int j;
	char c;
    t_quote_state state;
    bool has_heredoc;
	bool wait_more_args;
} t_var;

typedef struct s_v
{
	t_token	*new_token;
	t_token	*current;
	char	*buff;
	char	*joined;
	char 	*new_buff;
} t_v;
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


// typedef enum e_tp {
// 	txt,
// 	input,
// 	output,
// 	append,
// 	heredoc,
// } t_tp;



typedef struct s_redirection
{
	char					*file;
	int						type;
	int						fd;
	struct s_redirection	*next;
} t_redirection;

// Command before pipeline!!!!!
typedef struct s_cmd {
	char *name;
	char **args;
	int arg_count;
	int arg_capacity;
	bool	syn_err;
	t_redirection *in;
	t_redirection *out;
	struct s_cmd *next; // Next command in pipeline
} t_cmd;


// typedef struct s_arg
// {
// 	char *value;                     // Argument value
// 	bool need_expand;                // Needs variable expansion
// 	bool wait_more_args;			// Wait for more arguments
// 	t_tp	type;            // Argument type (text or file)
// } t_arg;


// /**
//  * Command structure
//  */
// typedef struct s_cmd {
//     char *name;                  // Command name
//     t_arg **args;                // Arguments array

//     int arg_count;               // Number of arguments
//     int arg_capacity;            // Capacity of arguments array
//     bool syn_err;					// Syntax error flag
//     struct s_cmd *next;          // Next command (after pipe)
// } t_cmd;

/*
 * String manipulation functions
 */
int	ft_strcmp(const char *s1, const char *s2);
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t  ft_strlen(const char *s);
char    **ft_split(char const *s, char c);
char    *ft_strdup(const char *s);
char    *ft_strchr(const char *s, int c);
int		ft_isspace(int c);
char	*ft_strjoin(const char *s1, const char *s2);
// int		ft_isspace(int c);
// void		ft_bzero(void *s, size_t n);

/*
 * Variable expansion functions
 */
// int     expand_variables(t_token *tokens, t_quote_state *state, int exit_status);
// char    *expand_env_vars(char *str, t_quote_state *state, int exit_status);
// char    *get_var_value(char *var_name);

/*
 * Token and parsing functions
 */

// typedef struct s_arr
// {
//     char *str;
//     struct s_arr *next;
// } t_arr;

t_token *tokenize_input(char *line);
void    detect_file_tokens(t_token **tokens);
int     validate_syntax(t_token **tokens);
t_cmd   *parse_tokens(t_token *tokens);


// void free_token_list(t_token *start);
int is_token_separator(char c);
int add_token_with_type(t_token **tokens, char *buffer, t_quote_state *state, bool wait_more_args);
int check_operator_validity(char *buffer, t_quote_state *state);
char *remove_character(char *str, char c);
void add_error_token(t_token **token);
void check_if_needs_expansion(t_token **tokens, t_quote_state state);
/*
 * Memory management functions
 */
void    free_token_list(t_token **start);
void    free_command(t_cmd *cmd);
void    free_commands(t_cmd *commands);


// int expand_variables(t_token *tokens, t_quote_state *state);
char *expand_env_vars(char *str, t_quote_state *state);

// char		*handle_quotes(char *buffer, t_quote_state *state);
// int			create_new_token(t_token **new_token, char *new_buff);
// void		add_to_token_list(t_token **tokens, t_token *new_token);
// void		check_if_needs_expansion(t_token **tokens, t_quote_state state);

// tokenize fill 
void first_condtion(t_var *v);
int	second_condition (t_var *v);
int third_condition(t_var *v);
int Fourth_condition(t_var *v, char *line);
int fill1_fourth(t_var *v, char *line);
int fill2_fourth(t_var *v, char *line);
int fill3_fourth(t_var *v);
int fill_fill3_fourth(t_var *v);
int fifth_condition(t_var *v);
int sixth_condition(t_var *v);

//tkenizer utils


#endif /* HE_H */