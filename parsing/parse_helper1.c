#include "parsing.h"

t_cmd *create_new_command(t_sp_var *va)
{
    t_cmd *cmd = mmallocc(sizeof(t_cmd), &va->allocs, P_GARBAGE);
    if (!cmd)
        return NULL;
    
    cmd->name = NULL;
    cmd->arg_count = 0;
    cmd->arg_capacity = 10;
    cmd->args = mmallocc(sizeof(char *) * (cmd->arg_capacity + 1), &va->allocs, P_GARBAGE);
    
    if (!cmd->args) {
        free(cmd);
        return NULL;
    }

	int i = -1;
    while (++i <= cmd->arg_capacity)
        cmd->args[i] = NULL;
    
	cmd->am = false;
    cmd->in = NULL;
    cmd->out = NULL;
    cmd->next = NULL;
    return cmd;
}

void add_argument(t_cmd *cmd, char *arg, t_sp_var *va)
{
    if (!cmd || !arg)
        return;

    if (cmd->arg_count == 0)
	{
        cmd->name = ft_strdup(arg, &va->allocs, P_GARBAGE);
	}
	if (!cmd->name)
		return;
    if (cmd->arg_count >= cmd->arg_capacity)
	{
        cmd->arg_capacity *= 2;
        char **new_args = mmallocc(sizeof(char *) * (cmd->arg_capacity + 1), &va->allocs, P_GARBAGE);
        if (!new_args)
            return;
		int i = -1;
        while (++i < cmd->arg_count)
            new_args[i] = cmd->args[i];
		i = cmd->arg_count - 1;
        while (++i <= cmd->arg_capacity)
            new_args[i] = NULL;
        cmd->args = new_args;
    }
    cmd->args[cmd->arg_count] = ft_strdup(arg, &va->allocs, P_GARBAGE);
    cmd->arg_count++;
    cmd->args[cmd->arg_count] = NULL;
}

t_redirection	*create_redirection(char *file, int type, t_sp_var *va)
{
	t_redirection	*redir;

	redir = mmallocc(sizeof(t_redirection), &va->allocs, P_GARBAGE);
	if (!redir)
		return NULL;
	redir->file = ft_strdup(file, &va->allocs, P_GARBAGE);
	redir->type = type;
	redir->fd = -11;
	redir->next = NULL;
	// redir->err_type = 0;
	return (redir);
}

void init_vpt(t_token *tokens, t_pt *vpt)
{
	vpt->commands = NULL;
	vpt->current_cmd = NULL;
	vpt->current = tokens;
	vpt->prev = NULL;
	vpt->s = -1;
	vpt->tmp_err = 0;
	vpt->err = 0;
	vpt->syn_err = false;
	vpt->err_file = NULL;
	vpt->result = -1;
	vpt->a = -1;
}

int ft_detect_syn_err(t_token *tokens, t_sp_var *va)
{
	while(va->vpt->current)
	{
		if(va->vpt->current->syn_err)
			va->vpt->syn_err = true;
		
		va->vpt->prev = va->vpt->current;
		va->vpt->current = va->vpt->current->next;
	}
	va->vpt->current = tokens;
	va->vpt->commands = create_new_command(va);
	if (!va->vpt->commands)
		return 0;
	
	va->vpt->current_cmd = va->vpt->commands;
	return (1);
}