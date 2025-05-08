
#include "he.h"

int fill1_fourth(t_var *v, char *line, t_env *env, int status)
{
	if (v->i > 0 && v->buffer[0] != '<' && v->buffer[0] != '>' && v->buffer[0] != '|')
	{
		v->buffer[v->i] = '\0';
		if (!add_token_with_type(v, env, status))
		{
			v->i = 0;
			return (0);
		}
		v->i = 0;
		return (1);
	}
	else if (v->i > 0 && v->buffer[0] == '<' && v->c == '<')
	{
		v->buffer[v->i++] = v->c;
		// v->buffer[v->i] = '\0';
		if (!line[v->j + 1] || line[v->j + 1] != '<')
		{
			// printf(":::::::::");
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(v, env, status))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			return (1);
		}
	}
	return (2);
}
int fill2_fourth(t_var *v, char *line, t_env *env, int status)
{
	if (v->i > 0 && v->buffer[0] == '>' && v->c == '>')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1] || line[v->j + 1] != '>')
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(v, env, status))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			return (1);
		}
	}
	else if (v->i > 0 && v->buffer[0] == '|' && v->c == '|')
	{
		v->buffer[v->i++] = v->c;
		if (!line[v->j + 1] || line[v->j + 1] != '|')
		{
			v->buffer[v->i] = '\0';
			if (!add_token_with_type(v, env, status))
			{
				v->i = 0;
				return (0);
			}
			v->i = 0;
			return (1);
		}
	}
	return (2);
}
// int	fill_fill3_fourth(t_var *v, t_env *env, int status)
// {
// 	v->buffer[v->i] = '\0';
// 	if (!add_token_with_type(v, env, status))
// 	{
// 		v->i = 0;
// 		return (0);
// 	}
// 	v->i = 0;
// 	return (1);
// }

// int fill3_fourth(t_var *v, t_env *env, int status)
// {
// 	if (v->i > 0 && v->buffer[0] == '<' && v->c != '<')
// 	{
// 		v->buffer[v->i] = '\0';
// 		if (!add_token_with_type(v, env, status))
// 		{
// 			v->i = 0;
// 			return (0);
// 		}
// 		v->i = 0;
// 		return (1);
// 	}
// 	else if (v->i > 0 && v->buffer[0] == '>' && v->c != '>')
// 	{
// 		v->buffer[v->i] = '\0';
// 		if (!add_token_with_type(v, env, status))
// 		{
// 			v->i = 0;
// 			return (0);
// 		}
// 		v->i = 0;
// 		return (1);
// 	}
// 	else if (v->i > 0 && v->buffer[0] == '|' && v->c != '|')
// 		return (fill_fill3_fourth(v, env, status));
// 	else
// 		return (2);
// }


int Fourth_condition(t_var *v, char *line, t_env *env, int status)
{
    int a;
    int b;
    v->wait_more_args = false;
    a = fill1_fourth(v, line, env, status);
    b = fill2_fourth(v, line, env, status);
    
    if (a != 2)
    {
        if (a == 0)
            return (0);
    }
    else if (b != 2)
    {
        if (b == 0)
            return (0);
    }
    // Add this condition to handle single redirection characters
    else if (v->i > 0 && (v->buffer[0] == '<' || v->buffer[0] == '>' || v->buffer[0] == '|'))
    {
        // Complete the current token
        v->buffer[v->i] = '\0';
        if (!add_token_with_type(v, env, status))
        {
            v->i = 0;
            return (0);
        }
        v->i = 0;
        
        // Only add the current character if it's not a space
        if (!ft_isspace(v->c))
            v->buffer[v->i++] = v->c;
            
        return (1);
    }
    else if (v->i == 0 && !ft_isspace(v->c))  // Only add non-space chars to empty buffer
        v->buffer[v->i++] = v->c;
        
    return (1);
}