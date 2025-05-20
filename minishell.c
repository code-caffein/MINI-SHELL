#include "./includes/minishell.h"
#include "./execution/execution.h"
int g_signal_pid = 0;

// void print_redirections(t_redirection *redir, const char *type) {
//     while (redir != NULL) {
//         printf("  %s: ", type);
//         switch (redir->type) {
//             case REDIR_IN:
//                 printf("INPUT <%s (fd=%d)\n", redir->file, redir->fd);
//                 break;
//             case REDIR_OUT:
//                 printf("OUTPUT >%s (fd=%d)\n", redir->file, redir->fd);
//                 break;
//             case REDIR_APPEND:
//                 printf("APPEND >>%s (fd=%d)\n", redir->file, redir->fd);
//                 break;
//             case REDIR_HEREDOC:
//                 printf("HEREDOC <<%s (fd=%d)\n", redir->file, redir->fd);
//                 break;
//             default:
//                 printf("UNKNOWN REDIRECTION\n");
//         }
//         redir = redir->next;
//     }
// }

// void print_commands(t_cmd *cmds) {
//     int cmd_num = 1;
//     while (cmds != NULL) {
//         printf("Command #%d:\n", cmd_num++);
//         // if (cmds->syn_err) {
//         //     printf("  [Syntax Error]\n");
//         //     cmds = cmds->next;
//         //     continue;
//         // }
// 		printf("\nname :%s\n\n", cmds->name);
//         // printf("  Name: %s\n", cmds->name ? cmds->name : "(null)");
//         printf("  Arguments:");
//         for (int i = 0; cmds->args && cmds->args[i]; i++) {
//             printf(" [%s]", cmds->args[i]);
//         }
//         printf("\n");
//         print_redirections(cmds->in, "Input Redirection");
//         print_redirections(cmds->out, "Output Redirection");
//         cmds = cmds->next;
//     }
// }


void init_v(t_sp_var *v)
{
	(v)->line = NULL;
	(v)->cmds = NULL;
	(v)->env = NULL;
	(v)->allocs = NULL;
	(v)->status = 0;
	v -> vpt = NULL;
}

void	init_env_variables(t_tools *tools, t_sp_var *v)
{
	char	*path;
	char	*shlvl;

	path = getcwd(NULL, 0);
	if (!path)
		perror("minshell-init: error retrieving current directory: getcwd");
	else if (path)
	{
		update_var(tools, ft_strdup(path, tools->aloc, 0), "p.a.t.h");
		update_var(tools, ft_strdup(path, tools->aloc, 0), "PWD");
	}
	free(path);
	path = get_key_value("OLDPWD", *(tools->env));
	update_var(tools, NULL, "OLDPWD");
	free_ptr((tools->aloc), path);
	shlvl = get_key_value("SHLVL", *(tools->env));
	if (shlvl)
		update_var(tools, ft_strdup(ft_itoa(ft_atoi(shlvl) + 1, v) \
		, tools->aloc, P_ENV), "SHLVL");
	else
		push_to_env(tools, "SHLVL", "1");
	path = get_key_value("PATH", *tools->env);
	if (!path)
		push_to_env(tools, "PATH", SECURE_PATH);
}

int	main(int argc, char **argv, char **envp)
{
	t_sp_var	*v;
	t_sp_var	s;
	t_tools		tools;

	(void)argc;
	(void)argv;
	v = &s;
	init_v(v);
	tools.cmd = NULL;
	tools.aloc = &(v->allocs);
	tools.r_stat = &(v->status);
	tools.env = &(v->env);
	tools.envp = envp;
	push_envp(&tools);
	init_env_variables(&tools, v);
	while (1)
	{
		signals(0);
		if (g_signal_pid == -1 || g_signal_pid == 4 || g_signal_pid == 5)
		{
			v->status = 1;
			g_signal_pid = 0;
		}
		if (!isatty(0) || !isatty(1))
			return (1);
		v->line = readline("minishell> ");
		if (!v->line)
			v->line = ft_strdup("exit", &v->allocs, P_GARBAGE);
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
				tools.cmd = v->cmds;
				ft_execute(&tools);
			}
			else if (v->status != 999)
				v->status = 258;
			else if (v->status == 999)
				v->status = 1;
			clean_garbage(tools.aloc);
		}
		//-----------
		//hna ktb li bghayti :(
			// printf("parse failed\n");
		//----------
		free(v->line);
	}

    return 0;
}
