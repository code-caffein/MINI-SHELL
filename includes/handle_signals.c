#include "minishell.h"

extern int g_signal_pid;



static void	handle_sigint(int n)
{
	(void)n;
	int status;
	int tmp;

	tmp = g_signal_pid;
	status = 0;
	if (g_signal_pid == 4)
		return ;
	if (waitpid(-1, &status, WNOHANG) == 0)
		return;
	if (status != 0)
		g_signal_pid = -1;
	else
		g_signal_pid = 0;
	if (tmp != 2)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
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

	struct sigaction sa;
sa.sa_handler = &handle_sigint;
sigemptyset(&sa.sa_mask);
sa.sa_flags = 0; // <-- no SA_RESTART!
sigaction(SIGINT, &sa, NULL);
	
		// signal(SIGINT, &handle_sigint);  // handle Ctrl+C
		signal(SIGQUIT, SIG_IGN);        // ignore Ctrl+\l
	
}
