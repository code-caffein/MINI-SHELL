#include "he.h"




t_cmd *parse(t_sp_var *v)
{
	t_token *tokens;
	t_cmd *commands;

	tokens = tokenize_input(v);
	if (!tokens)
		return (NULL);
	commands = parse_tokens(tokens, v);
	if(!commands)
		return (NULL);
	// if (commands)
	    // printf("zzz2yfs2t1ssytzdftftwdowuwu.\n");
	free_token_list(&tokens);
	return (commands);
}

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
//             printf(" %s", cmds->args[i]);
//         }
//         printf("\n");
//         print_redirections(cmds->in, "Input Redirection");
//         print_redirections(cmds->out, "Output Redirection");
//         cmds = cmds->next;
//     }
// }

// int main(void) {
//     char *input;

//     while (1) {
//         input = readline("demo_parser> ");
//         if (!input) {
//             printf("exit\n");
//             break;
//         }
//         if (*input != '\0') {
//             add_history(input);
//             t_cmd *commands = parse(input, NULL);
//             if (commands) {
//                 print_commands(commands);
//                 free_commands(commands);
//             } else {
//                 printf("Failed to parse or syntax error.\n");
//             }
//         }
//         free(input);
//     }
//     return 0;
// }