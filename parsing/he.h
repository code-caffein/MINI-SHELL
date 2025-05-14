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
#include "../includes/minishell.h"


typedef struct s_env t_env;
typedef struct s_cmd t_cmd;
typedef struct s_malloc		t_malloc;


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
    bool need_expand;       // heredoc expansion
	bool  newline;
    bool wait_more_args; // Wait for more arguments
    struct s_token *next;   // Next token in the list
} t_token;

typedef struct s_var
{
    t_token *tokens;
    char *buffer;
	char *tmp_err;
    int i;
    int j;
    char c;
	int a;
	int *p;
    t_quote_state state;
    bool has_heredoc;
    bool wait_more_args;
} t_var;

typedef struct s_v
{
    t_token *new_token;
    t_token *current;
    char    *buff;
    char    *joined;
    // bool quote;
    char    *new_buff;
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




typedef struct s_redirection
{
    char                    *file;
    int                     type;
    int                     fd;
    // int                      err_type;
    struct s_redirection    *next;
} t_redirection;

// Command before pipeline!!!!!
typedef struct s_cmd {
    char *name;
    char **args;
    int arg_count;
    int arg_capacity;
    // bool syn_err;
    t_redirection *in;
    t_redirection *out;
    struct s_cmd *next; // Next command in pipeline
} t_cmd;

typedef struct s_sp_var{
    char *line;
    t_cmd *cmds;
    t_env *env;
    t_malloc *allocs;
    int status;
    t_var *var;
} t_sp_var;
// typedef struct s_env {
//  char *key;
//  char *value;
//  int type;               // 0 for envp // 1 for my vars
//  struct s_env *next;
// } t_env;


/*
 * String manipulation functions
 */
// int  ft_strcmp(const char *s1, const char *s2);//
// size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
// size_t  ft_strlen(const char *s);//
// char    **ft_split(char const *s, char c);
// char    *ft_strdup(const char *s);
// char    *ft_strchr(const char *s, int c);
// int      ft_isspace(int c);
// char *ft_strjoin(const char *s1, const char *s2);//
// int      ft_isspace(int c);
// void     ft_bzero(void *s, size_t n);

/*
 * Variable expansion functions
 */
// int     expand_variables(t_token *tokens, t_quote_state *state, int exit_*status);
// char    *expand_env_vars(char *str, t_quote_state *state, int exit_*status);
// char    *get_var_value(char *var_name);

/*
 * Token and parsing functions
 */

// typedef struct s_arr
// {
//     char *str;
//     struct s_arr *next;
// } t_arr;

t_token *tokenize_input(t_sp_var *v);
void    detect_file_tokens(t_token **tokens);
int     validate_syntax(t_token **tokens);
t_cmd   *parse_tokens(t_token *tokens, t_sp_var *v);
t_cmd *parse(t_sp_var *v);


// void free_token_list(t_token *start);
int is_token_separator(char c);
// int add_token_with_type(t_token **tokens, char *buffer, t_quote_state *state, bool wait_more_args);
int add_token_with_type(t_sp_var *va);
int check_operator_validity(char *buffer, t_quote_state *state);
char *remove_character(t_sp_var *va, char c);
void add_error_token(t_token **token, t_sp_var *va);
void check_if_needs_expansion(t_token **tokens, t_quote_state state);
/*
 * Memory management functions
 */
// void    free_token_list(t_token **start);
// void    free_command(t_cmd *cmd);
// void    free_commands(t_cmd *commands);


// int expand_variables(t_token *tokens, t_quote_state *state);
// char *expand2_env_vars(char *str, t_quote_state *state, t_sp_var *v, t_malloc **a);
char *expand_env_vars(char *str, t_sp_var *va);

// char     *handle_quotes(char *buffer, t_quote_state *state);
// int          create_new_token(t_token **new_token, char *new_buff);
// void     add_to_token_list(t_token **tokens, t_token *new_token);
// void     check_if_needs_expansion(t_token **tokens, t_quote_state state);

// tokenize fill 
// void first_condtion(t_var *v, t_env *env, int status);
void first_condtion(t_sp_var *va);
// int  second_condition (t_var *v, t_env *env, int status);
int second_condition (t_sp_var *va);
// int third_condition(t_var *v, t_env *env, int status);
int third_condition(t_sp_var *va);
// int Fourth_condition(t_var *v, char *line, t_env *env, int status);
int Fourth_condition(t_sp_var *va);
// int fill1_fourth(t_var *v, char *line, t_env *env, int status);
int fill1_fourth(t_sp_var *va);
// int fill2_fourth(t_var *v, char *line, t_env *env, int status);
int fill2_fourth(t_sp_var *va);
// int fill3_fourth(t_var *v,t_env *env, int status);
int fill3_fourth(t_sp_var *va);
// int fill_fill3_fourth(t_var *v,t_env *env, int status);
int fill_fill3_fourth(t_sp_var *va);
// int fifth_condition(t_var *v, t_env *env, int status);
int fifth_condition(t_sp_var *va);
// int sixth_condition(t_var *v, t_env *env, int status);
int sixth_condition(t_sp_var *va);

//tkenizer utils


// char    *ft_sttrdup(const char *s1);
// char    *ft_sttrjoin(char const *s1, char const *s2);
// char    **ft_spplit(char const *s, char c);



int first_if(t_sp_var *va);
int second_if(t_sp_var *va);
int third_if(t_sp_var *va);
int fourth_if(t_sp_var *va);
int while_part1(t_sp_var *va);


void	init_var(t_sp_var *va);
bool	fill_tken_part1(t_var *v);
t_token *fill_tken_part2(t_var *v);
t_token *fill_tken_part3(t_var *v);


t_token *create_new_token(t_token **new_token, char *new_buff, t_sp_var *va);
void init_variable(t_v *v);
int ft_fill_2(t_v *v, t_sp_var *va);
int ft_fill_1(t_sp_var *va);
int need_expandd(char *str);

int fill1_red_pip_txt(t_v *v, t_quote_state *state);
void fill2_red_pip_txt(t_v *v, t_quote_state *state);
void red_pip_txt(t_v *v , t_quote_state *state);
void add_expanded_token(t_v *v, t_token **tokens, char *expanded, t_sp_var *va);
void add_token(t_v *v, t_token **tokens);

#endif /* HE_H */