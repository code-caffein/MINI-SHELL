/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:52:14 by aelbour           #+#    #+#             */
/*   Updated: 2025/04/30 17:41:09 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char *get_executable_path(char *str)
{
	int  i;
	char *s = getenv("PATH");
	char **paths = ft_split(s,':');
	char *check;

	i = -1;
	while(paths[++i])
	{
		check = ft_strjoin(paths[i], ft_strjoin("/", str));
		// printf("path checked = %s\n", check);
		if(!access(check, X_OK))
			return(check);
	}
	return(NULL);
}

int is_builtins(char *str)
{
	int i = 0;

	if(!ft_strcmp(str, "echo"))
		return(1);
	else if(!ft_strcmp(str, "cd"))
		return(2);
	else if(!ft_strcmp(str, "pwd"))
		return(3);
	else if(!ft_strcmp(str, "export"))
		return(4);
	else if(!ft_strcmp(str, "unset"))
		return(5);
	else if(!ft_strcmp(str, "env"))
		return(6);
	else if(!ft_strcmp(str, "exit"))
		return(7);
	return(0);
}

void	execute_parent_cmds(int i, t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{
	if(i == 1)
		*g_exit_status = ft_echo(cmd);
	else if(i == 2)
		*g_exit_status = ft_cd(cmd, allocs);
	else if(i == 3)
		*g_exit_status = ft_pwd(cmd, allocs);
	else if(i == 4)
		*g_exit_status = ft_export(cmd, allocs, env);
	else if(i == 5)
		*g_exit_status = ft_unset(cmd, allocs, env);
	else if(i == 6)
		*g_exit_status = ft_env(allocs, env, cmd);
	else if(i == 7)
		ft_exit(allocs, cmd, g_exit_status);
}

void get_a_child(int *g_exit_status, t_cmd *cmd, t_malloc **allocs, t_env **env)
{
	pid_t pid;
	int status;

	pid = fork();
	if(pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*g_exit_status = 128 + WTERMSIG(status);
	}
	else
	{
		if (execve(cmd->name, cmd->args, NULL) == -1)
			execve_error(cmd->name);
	}
}

int file_error_handler(char *path, int *status)
{
	struct stat	info;

	if (stat(path, &info) == 0)
	{
		if (S_ISDIR(info.st_mode) == true)
			cmd_file_error(path,"is a Directorie");
		else if (access(path, X_OK) == 0)
			return (1);
		else
			cmd_file_error(path,"permission denied");
		*status = 126;
	}
	else
	{
		cmd_file_error(path,"No such file or directory");
		*status = 127;
	}
	return (0);
}

void ft_execute(t_cmd *cmd, t_malloc **allocs, t_env **env, int *g_exit_status)
{
	int		i;
	char	*path;

	i = is_builtins(cmd->name);
	if (i)
		execute_parent_cmds(i, cmd, allocs, env, g_exit_status);
	else if (ft_strchr(cmd->name, '/'))
	{
		if (file_error_handler(cmd->name, g_exit_status))
			get_a_child(g_exit_status ,cmd, allocs, env);
	}
	else
	{
		path = get_executable_path(cmd->name);
		// printf("executable path = %s\n", path);
		if (path)
		{
			cmd->name = path;
			get_a_child(g_exit_status, cmd, allocs, env);
		}
		else
		{
			printf("minishell: %s: command not found\n", cmd->name);
			*g_exit_status = 127;
		}
	}
}

//main function for some cmd and builtins tests
// static void run_test(const char *description, t_cmd *cmd,
//                      t_malloc **alloc_list, t_env **env_list, int *status)
// {
//     printf("[TEST] %s\n", description);
//     ft_execute(cmd, alloc_list, env_list, status);
//     printf("--> status = %d\n\n", *status);
// }

// int main(int argc, char **argv, char **envp)
// {
//     t_cmd      cmd;
//     t_env     *env_list;
//     t_malloc  *alloc_list;
//     int        status;

//     // Initialize environment and allocations
//     env_list = NULL;
//     alloc_list = NULL;
//     status = 0;
//     push_envp(&env_list, envp);

//     // 1. Builtin tests
//     // echo
//     cmd.name = "echo";
//     cmd.args = ft_split("echo Hello World", ' ');
//     run_test("echo Hello World (builtin)", &cmd, &alloc_list, &env_list, &status);

//     // cd with no args
//     cmd.name = "cd";
//     cmd.args = ft_split("cd", ' ');
//     run_test("cd (no args, builtin)", &cmd, &alloc_list, &env_list, &status);

//     // cd to non-existent
//     cmd.name = "cd";
//     cmd.args = ft_split("cd /no/such/path", ' ');
//     run_test("cd /no/such/path (error)", &cmd, &alloc_list, &env_list, &status);

//     // pwd
//     cmd.name = "pwd";
//     cmd.args = ft_split("pwd extra", ' ');
//     run_test("pwd extra (too many args)", &cmd, &alloc_list, &env_list, &status);
//     cmd.args = ft_split("pwd", ' ');
//     run_test("pwd (builtin)", &cmd, &alloc_list, &env_list, &status);

//     // export set
//     cmd.name = "export";
//     cmd.args = ft_split("export NEWVAR=foo", ' ');
//     run_test("export NEWVAR=foo (builtin)", &cmd, &alloc_list, &env_list, &status);

//     // export append
//     cmd.name = "export";
//     cmd.args = ft_split("export NEWVAR+=bar", ' ');
//     run_test("export NEWVAR+=bar (append)", &cmd, &alloc_list, &env_list, &status);

//     // export invalid
//     cmd.name = "export";
//     cmd.args = ft_split("export 1BAD=val", ' ');
//     run_test("export 1BAD=val (invalid)", &cmd, &alloc_list, &env_list, &status);

    // // export display
    // cmd.name = "export";
    // cmd.args = ft_split("export", ' ');
    // run_test("export (display)", &cmd, &alloc_list, &env_list, &status);

    // // unset existing
    // cmd.name = "unset";
    // cmd.args = ft_split("unset NEWVAR", ' ');
    // run_test("unset NEWVAR (builtin)", &cmd, &alloc_list, &env_list, &status);

    // // unset invalid
    // cmd.name = "unset";
    // cmd.args = ft_split("unset 1BAD", ' ');
    // run_test("unset 1BAD (invalid)", &cmd, &alloc_list, &env_list, &status);

    // // env display
    // cmd.name = "env";
    // cmd.args = ft_split("env", ' ');
    // run_test("env (builtin)", &cmd, &alloc_list, &env_list, &status);

    // // env with args
    // cmd.name = "env";
    // cmd.args = ft_split("env arg1", ' ');
    // run_test("env arg1 (too many args)", &cmd, &alloc_list, &env_list, &status);

    // // exit with too many args (builtin, does not exit)
    // cmd.name = "exit";
    // cmd.args = ft_split("exit 1 2", ' ');
    // run_test("exit 1 2 (too many args)", &cmd, &alloc_list, &env_list, &status);

    // // exit with non-numeric
    // cmd.args = ft_split("exit abc", ' ');
    // // Note: this will exit the shell; comment out if you want to continue tests
    // // run_test("exit abc (numeric required)", &cmd, &alloc_list, &env_list, &status);

    // // 2. External commands
    // // PATH resolution
    // cmd.name = "ls";
    // cmd.args = ft_split("ls -la", ' ');
    // run_test("ls -la (external)", &cmd, &alloc_list, &env_list, &status);

    // // absolute path
    // cmd.name = "/bin/echo";
    // cmd.args = ft_split("/bin/echo hello absolute", ' ');
    // run_test("/bin/echo hello absolute (external)", &cmd, &alloc_list, &env_list, &status);

    // relative path (helper binary must exist)
    // cmd.name = "./helper";
    // cmd.args = ft_split("./helper test", ' ');
    // run_test("./helper test (relative external)", &cmd, &alloc_list, &env_list, &status);

//     return (0);
// }
