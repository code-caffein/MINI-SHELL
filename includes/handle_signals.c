#include "minishell.h"

extern int g_signal_pid;



static void	handle_sigint(int n)
{
	(void)n;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_signal_pid == 0)
		rl_redisplay();
}




void	signals(void)
{
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

// ctrl + D : when redline return null (generate a function that clean and exit)!!!!!!!!!!!!!!!