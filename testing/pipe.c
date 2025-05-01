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
	struct s_cmd *next; // Next command in pipeline
} t_cmd;
git 
void simple_pipe(t_cmd *cmd, int fd[])
{
	t_cmd *cmd2 = cmd->next;
	int pid;

	pid = fork();
	if (pid > 0)
	{
		dup2(fd[1] , 1);
		close(fd[0]);
		ft_execute(cmd);
	}
	else
	{
		dup2(fd[0] , 0);
		close(fd[1]);
		ft_execute(cmd2);
	}
}

void run_all_pipes(t_cmd *cmd)
{
	int fd[2];

	while(cmd->next)
	{
		pipe(fd);

	}
}
void execute_cmd_line(t_cmd *cmd)
{
	int fd[2];
	int pid;
	int status;

	if(cmd->next)
	{
		pipe(fd);
		pid = fork();
		if(pid > 0)
		{
			waitpid(pid, &status, 0);
		}
		else
		{
			simple_pipe(cmd, fd);
		}
	}
	else
	{
		ft_execute(cmd);
	}
}


int main(void)
{
	t_cmd *cmd1 = malloc(sizeof(t_cmd));
	t_cmd *cmd2 = malloc(sizeof(t_cmd));

	cmd1->name = "ls";
	cmd1->args = ft_split("ls -la", ' ');
	cmd1->next = cmd2;

	cmd2->name = "cat";
	cmd2->args = ft_split("cat", ' ');
	cmd2->next = NULL;
	execute_cmd_line(cmd1);
}
