/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:34:46 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/18 15:34:38 by abel-had         ###   ########.fr       */
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
	bool wait_more_args;			// Wait for more arguments
	t_tp	type;            // Argument type (text or file)
} t_arg;


/**
 * Command structure
 */
typedef struct s_cmd {
    char *name;                  // Command name
    t_arg **args;                // Arguments array

    int arg_count;               // Number of arguments
    int arg_capacity;            // Capacity of arguments array
    bool syn_err;					// Syntax error flag
    struct s_cmd *next;          // Next command (after pipe)
} t_cmd;

size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t  ft_strlen(const char *s);
char    **ft_split(char const *s, char c);
char    *ft_strdup(const char *s);
char    *ft_strchr(const char *s, int c);


#endif