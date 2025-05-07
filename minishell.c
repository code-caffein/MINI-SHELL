#include "./includes/minishell.h"
#include "./execution/execution.h"

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


int	main(int argc, char **argv, char **envp)
{
	char *input;
	t_cmd *cmds;
	t_env *env;
	t_malloc *allocs;
	int status;

	status = 0;
	push_envp(&env, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			// printf("exit\n");
			//generate a function that print exit\n and exit (ctrl + d)
		}
		printf("[%d]\n",status);
		if (input[0] != '\0')
			add_history(input);
		if (input[0] != '\n')
		{
			cmds = parse(input, env, status);
		// print_commands(cmds);
        // free_commands(cmds);
			if (cmds)
			{
				// print_commands(cmds);
        		// free_commands(cmds);
				ft_execute(cmds, &status, &allocs, &env);
			}
		}
		//-----------
		//hna ktb li bghayti :(
			// printf("parse failed\n");
		//----------
		free(input);
    }

    return 0;
}
