#include "minishell.h"

extern volatile sig_atomic_t g_signal_pid;



static void	handle_sigint(int n)
{
	(void)n;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_signal_pid == 0)
	{
		rl_redisplay();
		g_signal_pid = -1;
	}
}




void	signals(void)
{
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

// ctrl + D : when redline return null (generate a function that clean and exit)!!!!!!!!!!!!!!!