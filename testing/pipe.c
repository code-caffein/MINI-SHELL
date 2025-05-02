#include <libc.h>

typedef struct s_redirection
{
	char					*file;
	int						type;
	int						fd;
	struct s_redirection	*next;
} t_redirection;

typedef struct s_cmd {
	char *name;
	char **args;
	int arg_count;
	int arg_capacity;
	int	syn_err;
	t_redirection *in;
	t_redirection *out;
	struct s_cmd *next;
} t_cmd;

int **get_pipe_buff(t_cmd *cmd)
{
	int n;
	int **arr;
	n = 0;
	while(cmd)
	{
		cmd = cmd->next;
		n++;
	}
	arr = malloc(sizeof(int *) * (n - 1));
	while(--n > 0)
	{
		arr[n - 1] = malloc(2* sizeof(int));
		pipe(arr[n - 1]);
	}
	return(arr);
}

void execute_cmd_line(t_cmd *cmd)
{
	int pid;
	int status;
	int **arr;
	int num;

	num = 0;
	arr = get_pipe_buff(cmd);
	if(cmd->next)
	{
		while(cmd)
		{
			pid = fork();
			if (pid == 0)
			{
				if(cmd->next)
					dup2(arr[num][1], STDOUT_FILENO);
				else if(num)
					dup2(arr[num - 1][0], STDIN_FILENO);
			}
			cmd = cmd->next;
			num++;
		}
		execute_piped_cmd(cmd);
	}
	else
	{
		ft_execute(cmd);
	}
	waitpid(pid, &status, 0);
	while (wait(NULL) ==  -1);
}

int main(void)
{
	t_cmd *cmd1 = malloc(sizeof(t_cmd));
	t_cmd *cmd2 = malloc(sizeof(t_cmd));
	t_cmd *cmd3 = malloc(sizeof(t_cmd));


	cmd1->name = "ls";
	cmd1->args = ft_split("ls -la", ' ');
	cmd1->next = cmd2;

	cmd2->name = "cat";
	cmd2->args = ft_split("cat", ' ');
	cmd2->next = cmd3;

	cmd3->name = "wc";
	cmd3->args = ft_split("wc -l", ' ');
	cmd3->next = NULL;
	execute_cmd_line(cmd1);
}
