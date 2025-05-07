#include "./includes/minishell.h"
#include "./execution/execution.h"

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

		if (input[0] != '\0')
			add_history(input);
		cmds = parse(input, env);
		if (!cmds)
			printf("parse failed\n");
		//-----------
		//hna ktb li bghayti :(
		//----------
		free(input);
    }

    return 0;
}
