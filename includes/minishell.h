/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:34:46 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/24 19:40:24 by aelbour          ###   ########.fr       */
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
#include <stddef.h>

typedef enum e_tp {
	text,
	input,
	output,
	append,
	heredoc,
} t_tp;

typedef struct s_arg
{
	char			*value;                     // Argument value
	bool			need_expand;                // Needs variable expansion
	bool			wait_more_args;			// Wait for more arguments
	t_tp			type;  
	struct s_arg	*next;          // Next command (after pipe)
			  // Argument type (text or file)
} t_arg;

/**
 * Command structure
 */
typedef struct s_cmd {
	char *name;                  // Command name
	char **args;                // Arguments array

	int arg_count;               // Number of arguments
	int arg_capacity;            // Capacity of arguments array
	bool syn_err;					// Syntax error flag
	struct s_cmd *next;          // Next command (after pipe)
} t_cmd;

typedef struct s_malloc {
	void            *ptr;   // the allocated block
	struct s_malloc  *next;	
}               t_malloc;

typedef struct s_env {
	char *key;
	char *value;
	struct s_env *next;
} t_env;


size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t  ft_strlen(const char *s);
char    **ft_split(char const *s, char c);
char    *ft_strdup(const char *s);
char    *ft_strchr(const char *s, int c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int	count_list_size(void *head, size_t offset);
int	ft_strcmp(const char *s1, const char *s2);


#endif