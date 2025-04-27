#include "he.h"

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <libc.h>

int main() {
    char *input;

    // Prompt the user for input
    input = readline("> ");
    
    if (input) {
        // User typed 'helllo', and we want to correct it automatically
        if (strcmp(input, "helllo") == 0) {
            rl_replace_line("hello", 0); // Replace with the corrected version
            rl_redisplay(); // Refresh the display to show the change
        }
    }
    
    return 0;
}
