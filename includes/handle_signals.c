#include "minishell.h"

extern int g_signal_pid;



static void handle_sigint(int n)
{
    (void)n;
    int status;

//    static int in_heredoc = 0;

    if (g_signal_pid == 2) {
        // in_heredoc = 1;
        g_signal_pid = -2;
        return;
    }
	if (g_signal_pid == -2)
	{
		write(1, "\n", 1);
    	rl_replace_line("", 0);
    	rl_on_new_line();
		rl_redisplay();
        // in_heredoc = 0;
        return;
    }

    if (g_signal_pid == 4)
        return;

    status = 0;
    if (waitpid(-1, &status, WNOHANG) == 0)
        return;
    if (status != 0)
        g_signal_pid = -1;
    else
        g_signal_pid = 0;

    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();

    if (g_signal_pid == 0)
    {
        rl_redisplay();
        g_signal_pid = -1;
    }
}

void signals(int heredoc_mode)
{
    struct sigaction sa;

    sa.sa_handler = &handle_sigint;
    sigemptyset(&sa.sa_mask);
    
  
    if (heredoc_mode)
        sa.sa_flags = 0;
    else
        sa.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
