#include "he.h"

int g_signal_pid;

int heredoc_readline(char **out, t_sp_var *va)
{
    int cap = 128, len = 0;
    char *buf = mmallocc(cap, &va->allocs, P_GARBAGE);
    if (!buf) return -1;
	
    while (1)
    {
		char c;
		int r = read(0, &c, 1);   
		if (r == 0)     // EOF
			return 0;
        if (r < 0)
    {
        if (errno == EINTR) {
                if (g_signal_pid == -1) {
                    // Flush pending input using ioctl and read
                    int bytes_available;
                    while (1) {
                        if (ioctl(0, FIONREAD, &bytes_available) == -1 || bytes_available <= 0)
                            break;
                        char dummy[256];
                        if (read(0, dummy, sizeof(dummy)) <= 0)
                            break;
                    }
                }
					rl_replace_line("", 0);
            		rl_redisplay();
                    // free(buf);
                    *out = NULL;
                    return -2;
            }
        // free(buf);
        return -1;
    }

        // append c
        if (len + 1 >= cap)
        {
            cap *= 2;
            char *t = realloc(buf, cap);
            if (!t) { 
				// free(buf);
				return -1; 
			}
            buf = t;
        }
        buf[len++] = c;
        if (c == '\n')   break;
    }
	

    buf[len] = '\0';
    *out = buf;
	// printf("[%s]\n",*out);
    return (ssize_t)len;
}