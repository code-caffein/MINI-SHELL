#include "minishell.h"

extern volatile sig_atomic_t g_signal_pid;



static void	handle_sigint(int n)
{
	(void)n;
	int status = 0;
    
    // Non-volatile status for waitpid
    if (waitpid(-1, &status, WNOHANG) == 0)
        return;

    // Set the global signal status based on the waitpid result
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



#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <termios.h>

void	signals(void)
{
	
		signal(SIGINT, &handle_sigint);  // handle Ctrl+C
		signal(SIGQUIT, SIG_IGN);        // ignore Ctrl+\l
	
}


// ctrl + D : when redline return null (generate a function that clean and exit)!!!!!!!!!!!!!!!