/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:34:46 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/17 18:01:38 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

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

typedef struct s_malloc {
	void 			*ptr; // where the addresses of heap a
	struct s_alloc	*next;
} t_malloc;

typedef struct s_token {
	char *value;            // Token content
    t_types type;           // Token type
    bool syn_err;           // Syntax error flag
    bool heredoc;           // Is heredoc delimiter
    bool need_expand;       // Needs variable expansion
	bool wait_more_args; // Wait for more arguments
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

typedef enum e_tp {
	text,
	input,
	output,
	append,
	heredoc,
} t_tp;

typedef struct s_arg
{
	char *value;                     // Argument value
	bool need_expand;                // Needs variable expansion
	bool wait_more_args;
	t_tp	type;            // Argument type (text or file)
} t_arg;

/**
 * Command structure
 */
typedef struct s_cmd {
    char *name;                  // Command name
    t_arg **args;                // Arguments array
    // bool *args_need_expand;      // Flags for which arguments need expansion

    int arg_count;               // Number of arguments
    int arg_capacity;            // Capacity of arguments array
    bool syn_err;					// Syntax error flag
    struct s_cmd *next;          // Next command (after pipe)
} t_cmd;

/*
 * String manipulation functions
 */
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t  ft_strlen(const char *s);
char    **ft_split(char const *s, char c);
char    *ft_strdup(const char *s);
char    *ft_strchr(const char *s, int c);


#endif