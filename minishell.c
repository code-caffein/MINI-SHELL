#include "./includes/minishell.h"
#include "./execution/execution.h"
#
int	main(int argc, char **argv, char **envp)
{
	char input;
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

		if (input != '\0')
			add_history(input);
		cmds = parse(input); 
		//-----------
		//hna ktb li bghayti :(
		ft_execute(cmds, &status, &allocs, &env);
		//----------
		free(input);
    }

    return 0;
}
