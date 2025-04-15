# include "he.h"
//0 = true  , 1 = false $?
extern int var_status;


char *get_var_value(char *var_name)
{
    if (!var_name)
        return NULL;

    char *env_value = getenv(var_name);
    if (env_value)
        return ft_strdup(env_value);
    
    return ft_strdup("");
}


char *expand_env_vars(char *str, t_quote_state *state)
{
    if (!str)
        return NULL;
    
    int i = 0;
    int j = 0;
    char result[9999] = {0};
    
    while (str[i])
    {
        if (str[i] == '$' && str[i+1] && state != SINGLE_QUOTED)
        {
            i++;
            if (str[i] == '?')
            {
                char c;
                if(!var_status)
					c = '0';
				else
					c = '1';
                result[j++] = c;
                
                i++;
                continue;
            }
            
            char var_name[256] = {0};
            int k = 0;
            
            while (str[i] && ((str[i] >= 'a' && str[i] <= 'z') ||
                             (str[i] >= 'A' && str[i] <= 'Z') ||
                             (str[i] >= '0' && str[i] <= '9') ||
                             str[i] == '_'))
                var_name[k++] = str[i++];
            
            var_name[k] = '\0';
            
            char *value = get_var_value(var_name);
            if (value)
            {
                int m = 0;
                while (value[m])
                    result[j++] = value[m++];
                free(value);
            }
        }
        else
            result[j++] = str[i++];
    }
    
    result[j] = '\0';
    return ft_strdup(result);
}

int expand_variables(t_token *tokens, t_quote_state *state)
{
    t_token *current = tokens;

    while (current)
    {
        if (current->type == text || current->type == file)
        {
            char *expanded = expand_env_vars(current->value, state);
            if (!expanded)
                return 0; 
            free(current->value);
            current->value = expanded;
        }
        current = current->next;
    }
    return (1);
}



///////////////////////??????