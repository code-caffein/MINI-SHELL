#include "minishell.h"

extern int g_signal_pid;



static void	handle_sigint(int n)
{
	(void)n;
	int status;

	status = 0;
	if (g_signal_pid == 4)
		return ;
	if (waitpid(-1, &status, WNOHANG) == 0)
		return;
	if (status != 0)
		g_signal_pid = -1;
	else
		g_signal_pid = 0;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_signal_pid == 0)
	{
		rl_redisplay();
		g_signal_pid = -1;
	}
	else
		g_signal_pid = -1;
}


void	signals(void)
{
	
		signal(SIGINT, &handle_sigint);  // handle Ctrl+C
		signal(SIGQUIT, SIG_IGN);        // ignore Ctrl+\l
	
}
