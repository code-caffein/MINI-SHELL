# ifndef HE_H
#define HE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef enum e_types
{pip, red, file, text} t_types;

typedef enum e_quote_state {
    UNQUOTED,
    SINGLE_QUOTED,
    DOUBLE_QUOTED
} t_quote_state;



typedef struct s_token
{
	char *value;
	t_types type;
	bool syn_err;
	bool heredoc;
	bool need_expand;
	struct s_token *next;
} t_token;


// typedef struct  s_tree
// {
// 	char	*right;
// 	char	*left;
// }	t_tree;
#define REDIR_IN 1      // <
#define REDIR_OUT 2     // >
#define REDIR_APPEND 3  // >>
#define REDIR_HEREDOC 4 // <<

typedef struct s_redirection
{
	char					*file;
	int						type;
	struct s_redirection	*next;
} t_redirection;


typedef struct s_cmd 
{
	char			*name;
	char			**args;
	bool			*args_need_expand;
	int				arg_count;
	int 			arg_capacity;
	bool			syn_err;
	// bool			expand;
	t_redirection	*in;
	t_redirection	*out;
	struct s_cmd	*next; // next command after pipe or red !!!
} t_cmd;


int	ft_strcmp(const char *s1, const char *s2);

//split
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);

int ft_isspace(char c);

int expand_variables(t_token *tokens, t_quote_state *state, int exit_status);
char *expand_env_vars(char *str, t_quote_state *state, int exit_status);
char *get_var_value(char *var_name);
void detect_file(t_token *tokens);

int syntax_valid(t_token *tokens);



#endif