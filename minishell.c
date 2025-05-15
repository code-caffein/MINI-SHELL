#include "./includes/minishell.h"
#include "./execution/execution.h"
volatile sig_atomic_t g_signal_pid = 0;

void print_redirections(t_redirection *redir, const char *type) {
    while (redir != NULL) {
        printf("  %s: ", type);
        switch (redir->type) {
            case REDIR_IN:
                printf("INPUT <%s (fd=%d)\n", redir->file, redir->fd);
                break;
            case REDIR_OUT:
                printf("OUTPUT >%s (fd=%d)\n", redir->file, redir->fd);
                break;
            case REDIR_APPEND:
                printf("APPEND >>%s (fd=%d)\n", redir->file, redir->fd);
                break;
            case REDIR_HEREDOC:
                printf("HEREDOC <<%s (fd=%d)\n", redir->file, redir->fd);
                break;
            default:
                printf("UNKNOWN REDIRECTION\n");
        }
        redir = redir->next;
    }
}

void print_commands(t_cmd *cmds) {
    int cmd_num = 1;
    while (cmds != NULL) {
        printf("Command #%d:\n", cmd_num++);
        // if (cmds->syn_err) {
        //     printf("  [Syntax Error]\n");
        //     cmds = cmds->next;
        //     continue;
        // }
		printf("\nname :%s\n\n", cmds->name);
        // printf("  Name: %s\n", cmds->name ? cmds->name : "(null)");
        printf("  Arguments:");
        for (int i = 0; cmds->args && cmds->args[i]; i++) {
            printf(" [%s]", cmds->args[i]);
        }
        printf("\n");
        print_redirections(cmds->in, "Input Redirection");
        print_redirections(cmds->out, "Output Redirection");
        cmds = cmds->next;
    }
}

void init_v(t_sp_var *v)
{
	(v)->line = NULL;
	(v)->cmds = NULL;
	(v)->env = NULL;
	(v)->allocs = NULL;
	(v)->status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_sp_var *v;
	t_sp_var s;
	v =  &s;
	char *path;

	init_v(v);
	signals();

	push_envp(&(v->env), envp, &(v->allocs));
	path = get_key_value("PWD", s.env);
	push_to_env(&(s.env), &(s.allocs), "p.a.t.h", path, 3);
	path = get_key_value("OLDPWD", s.env);
	update_var(&(s.env), NULL, "OLDPWD", &(s.allocs));
	free_ptr(&(s.allocs), path);
	while (1)
	{
		if (g_signal_pid == -1)
    	{
        	v->status = 1;
        	g_signal_pid = 0;
    	}
		if (!isatty(0) || !isatty(1))
			return (1);
		v->line = readline("minishell> ");
		// printf("[%s]\n",v->line);
		if (!v->line)
        	v->line = ft_strdup("exit", &v->allocs, P_GARBAGE);
		// printf("[%d]\n",status);
		if (v->line[0] != '\0')
			add_history(v->line);
		if (g_signal_pid == -1)
    	{
        	v->status = 1;
        	g_signal_pid = 0;
        	// free(v->line);
        	// continue;
    	}
		if (*v->line != '\0')
		{
			// printf("\n[%c]\n",*input);
			// printf("dddddd\n");
			v->cmds = parse(v);
		// print_commands(cmds);
        // free_commands(cmds);
		//check 
			if (v->cmds )
			{
				// print_commands(v->cmds);
				// printf("hh\n");
        		// free_commands(cmds);
				// printf("---------------------\n");
				ft_execute(v->cmds, &v->status, &v->allocs, &v->env, envp);
			}
			else
				v->status = 258;
			
		}
		//-----------
		//hna ktb li bghayti :(
			// printf("parse failed\n");
		//----------
		free(v->line);
    }

    return 0;
}
