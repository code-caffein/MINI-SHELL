#include "minishell.h"

extern int g_signal_pid;



static void handle_sigint(int n)
{
    (void)n;
    int status;
    int tmp = g_signal_pid;

   static int in_heredoc = 0;

    if (g_signal_pid == 2) {  // Heredoc mode
        in_heredoc = 1;
        g_signal_pid = -2;    // Special code for heredoc interruption
        // write(1, "\n", 1);    // Ensure we get a new line
        return;
    }
	if (in_heredoc) {
        in_heredoc = 0;
        return;
    }

    if (g_signal_pid == 4 || g_signal_pid == 5)  // Child process or special case
        return;

    status = 0;
    if (waitpid(-1, &status, WNOHANG) == 0)
        return;

    if (status != 0)
        g_signal_pid = -1;
    else
        g_signal_pid = 0;

    if (tmp != 5 && g_signal_pid != 5)
    {
        write(1, "\n", 1);
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

    if (tmp == 5)
        g_signal_pid = 5;
}

void signals(int heredoc_mode)
{
    struct sigaction sa;

    sa.sa_handler = &handle_sigint;
    sigemptyset(&sa.sa_mask);
    
    // In heredoc mode, we don't want SA_RESTART to allow interrupting read()
    sa.sa_flags = heredoc_mode ? 0 : SA_RESTART;
    
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
