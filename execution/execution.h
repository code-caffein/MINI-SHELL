#ifndef EXECUTION_H
#define EXECUTION_H


#include<stdio.h> 
#include<unistd.h> 
#include <sys/stat.h>
#include "../includes/minishell.h"

char *get_executable_path(char *str);
char **extract_args(t_arg *arg);



#endif