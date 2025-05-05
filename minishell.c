#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char input;
	t_cmd *cmds;

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

		//----------
		free(input);
    }

    return 0;
}
