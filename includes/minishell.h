/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:34:46 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/30 17:47:22 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stddef.h>

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


typedef struct s_malloc {
	void            *ptr;   // the allocated block
	struct s_malloc  *next;	
}               t_malloc;

typedef struct s_env {
	char *key;
	char *value;
	int type;				// 0 for envp // 1 for my vars
	struct s_env *next;
} t_env;

char	*ft_isnum(char *s);
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t  ft_strlen(const char *s);
char    **ft_split(char const *s, char c);
char    *ft_strdup(const char *s);
char    *ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int	count_list_size(void *head, size_t offset);
int	ft_strcmp(const char *s1, const char *s2);
void clean_up(t_malloc **head);
void *mmallocc(size_t size, t_malloc **head);
int	ft_isalnum(unsigned char c);
int	ft_isalpha(int c);
int	ft_isdigit(int c);
void	ft_putstr_fd(char *s, int fd);
void swap_string(char **s1, char **s2);
void	*push_to_env(t_env **head, char *key, char *value, int type);
int count_env_nodes(t_env *env, int type);
void push_envp(t_env **head ,char **envp);
int	ft_atoi(const char *str);

#endif
