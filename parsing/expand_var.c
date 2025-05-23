#include "parsing.h"
//0 = true  , 1 = false $?
int g_signal_pid;


char *get_var_value(char *var_name, t_env *env, t_sp_var *va)
{
    if (!var_name)
        return NULL;

    char *env_value = get_key_value(var_name, env);
    if (env_value)
        return ft_strdup(env_value, &va->allocs, P_GARBAGE);
    
    return ft_strdup("", &va->allocs, P_GARBAGE);
}


char *expand_env_vars(char *str, t_sp_var *va)
{
    if (!str)
        return NULL;
    
    int i = 0;
    int j = 0;
	char *tmp;
    char result[9999] = {0};
	int m;

	
    while (str[i])
    {
        if ( str[i] == '$' && str[i+1] && va->var->state != SINGLE_QUOTED)
        {
			if ((str[i + 1] < '0' || (str[i + 1] > '9' && str[i + 1] < 'A') ||
							 (str[i + 1] > 'Z' && str[i + 1] < '_') ||
							  (str[i + 1] > '_' && str[i + 1] < 'a') ||
							   str[i + 1] > 'z') && str[i + 1] != '$' && str[i + 1] != '?')
			{
				i++;
				result[j++] = '$';
				continue;
			}
            i++;
            if (str[i] == '?')
            {
				m = 0;
				if (g_signal_pid == 0)
				{
					va->status = va->status;
				}
				else if (g_signal_pid == -1 ||  g_signal_pid -2 ||  g_signal_pid == 2)
				{
					va->status = 1;
					g_signal_pid = 0;
				}
				tmp = ft_itoa(va->status, va);
				if (!tmp)
					return NULL;
            	while (tmp[m] != '\0')
                	result[j++] = tmp[m++];
            	i++;
            	continue;
            }
			if (str[i] == '$')
			{
				m = 0;
				tmp = ft_itoa(getpid(), va);
				if (!tmp)
					return NULL;
            	while (tmp[m] != '\0')
                	result[j++] = tmp[m++];
            	i++;
            	continue;
			}
            
            char var_name[256] = {0};
            int k = 0;
            if (str[i] && (str[i] >= '0' && str[i] <= '9'))
			{
				i++;
				continue;
			}
				
            while (str[i] && ((str[i] >= 'a' && str[i] <= 'z') ||
                             (str[i] >= 'A' && str[i] <= 'Z') ||
                             (str[i] >= '0' && str[i] <= '9') ||
                             str[i] == '_'))
                var_name[k++] = str[i++];
            
            var_name[k] = '\0';
            
            char *value = get_var_value(var_name, va->env, va);
            if (value)
            {
                int m = 0;
                while (value[m])
                    result[j++] = value[m++];
                // free(value);
            }
        }
        else
            result[j++] = str[i++];
    }
    
    result[j] = '\0';
    return ft_strdup(result, &va->allocs, P_GARBAGE);
}
