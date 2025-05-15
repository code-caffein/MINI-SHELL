#include "minishell.h"

extern int g_signal_pid;



static void	handle_sigint(int n)
{
	(void)n;
	int status = 0;
    
	if (g_signal_pid == 2) {
		write(STDOUT_FILENO, "\n", 1);
		g_signal_pid = -1;  // Signal heredoc termination
		return;
	}
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

void signals(void) {
    struct sigaction sa_int;

    sa_int.sa_handler = &handle_sigint; // Your existing handler
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0; // Disable SA_RESTART to interrupt `read`
    sigaction(SIGINT, &sa_int, NULL); // Use sigaction instead of signal()

    signal(SIGQUIT, SIG_IGN);
}


// ctrl + D : when redline return null (generate a function that clean and exit)!!!!!!!!!!!!!!!