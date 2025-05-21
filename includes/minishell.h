/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:34:46 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/21 15:19:28 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
// #include <readline/readline.h>
// #include <readline/history.h>
# include <stddef.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../execution/execution.h"
# include "../parsing/parsing.h"

typedef struct s_env		t_env;
typedef struct s_cmd		t_cmd;
typedef struct s_malloc		t_malloc;
typedef struct s_tools		t_tools;
typedef struct s_sp_var		t_sp_var;	

# define SECURE_PATH "/mnt/homes/aelbour/tools/\
cmake-4.0.0-macos-universal/CMake.app/Contents/bi\
:/usr/local/bin:/usr/bin:/bin:\
/usr/sbin:/sbin:/usr/local/munki:\
/Library/Apple/usr/bin:\
/Library/Frameworks/Mono.framework/Versions/Current/Commands:\
/mnt/homes/aelbour/tools/cmake-4.0.0-macos-universal/CMake.app/Contents/bin" 

enum
{
	P_ENV,
	P_GARBAGE
};

typedef struct s_malloc
{
	void			*ptr;
	int				p_type;
	struct s_malloc	*next;
}	t_malloc;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

char			*ft_isnum(char *s, t_malloc **a);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t			ft_strlen(const char *s);
char			**ft_split(char const *s, char c, t_malloc **alloc);
char			*ft_strdup(const char *s1, t_malloc **alloc, int p_type);
char			*ft_strchr(const char *s, int c);
char			*ft_strjoin(char const *s1, char const *s2, t_malloc **a);
char			*ft_substr(char const *s, unsigned int start, \
							size_t len, t_malloc **a);
int				count_list_size(void *head, size_t offset);
int				ft_strcmp(const char *s1, const char *s2);
void			clean_up(t_malloc **head);
void			*mmallocc(size_t size, t_malloc **head, int p_type);
int				ft_isalnum(unsigned char c);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
void			ft_putstr_fd(char *s, int fd);
void			swap_string(char **s1, char **s2);
void			*push_to_env(t_tools *tools, char *key, char *value);
int				count_env_nodes(t_env *env);
void			push_envp(t_tools *tools);
long long int	ft_atoi(const char *str);
char			*get_key_value(char *key, t_env *env);
void			free_ptr(t_malloc **head, void *addr);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
int				ft_isspace(int c);
char			*ft_itoa(int n, t_sp_var *va);
void			signals(int heredoc_mode);
void			clean_garbage(t_malloc **aloc);
void			critical_error(char *syscall, t_malloc **alloc, int quit, int *r_stat);

#endif
