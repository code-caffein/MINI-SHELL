#include "parsing.h"

int while_part_if_1(t_sp_var *va)
{
	if (va->vpt->err != 0 && va->vpt->tmp_err != -2)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(va->vpt->err_file, 2);
		write(2,":",1);
		ft_putstr_fd(strerror(va->vpt->err), 2);
		write(2,"\n",1);
	}
	va->vpt->current_cmd->next = create_new_command(va);
	if (!va->vpt->current_cmd->next)
		return (0);
	va->vpt->s = 1;
	va->vpt->current_cmd = va->vpt->current_cmd->next;
	return (1);
}

int while_part_if_2_fill(t_sp_var *va)
{
	if (va->vpt->tmp_err == -2)
	{
		if (va->vpt->current_cmd && va->vpt->current_cmd->name)
		{
			va->vpt->current_cmd->name = NULL;
			return (2);
		}			
	}
	if (va->vpt->tmp_err != 0)
	{
		va->vpt->s = 0;
		if (va->vpt->current_cmd && va->vpt->current_cmd->name)
			va->vpt->current_cmd->name = NULL;
	}
	return (1);
}

int while_part_if_2(t_sp_var *va)
{
	if (!va->vpt->current->next || va->vpt->current->next->type != file)
	{
		ft_putstr_fd("minishell: syntax error: Missing filename after redirection\n", 2);
		return (0);
	}
	else if (va->vpt->current->next && va->vpt->current->next->type == file)
	{
		va->vpt->tmp_err = handle_redirection(va->vpt->current_cmd, va->vpt->current, va, va->vpt->s);
		if (va->vpt->tmp_err == -2)  // Heredoc was interrupted
        {
            // Skip remaining heredocs in this command
            while (va->vpt->current && va->vpt->current->type == red && 
                   va->vpt->current->next && va->vpt->current->next->type == file)
            {
                if (red_type(va->vpt->current->value) == REDIR_HEREDOC)
                    va->vpt->current = va->vpt->current->next;  // Skip the delimiter
                va->vpt->current = va->vpt->current->next;
            }
            return 10;  // Special code for interrupted heredoc
        }
		if (va->vpt->err == 0)
		{
			va->vpt->err = va->vpt->tmp_err;
			va->vpt->err_file = va->vpt->current->next->value;
		}
		if (while_part_if_2_fill(va) == 2)
			return (2);
		va->vpt->current = va->vpt->current->next;
	}
	return (1);
}

void	while_part_print_err(t_sp_var *va)
{
	if (va->vpt->err != 0 && va->vpt->tmp_err != -2)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(va->vpt->err_file, 2);
		write(2,":",1);
		ft_putstr_fd(strerror(va->vpt->err), 2);
		write(2,"\n",1);
	}
}

int	while_part(t_sp_var *va)
{
	while (va->vpt->current && !va->vpt->current->syn_err)
	{
		if (va->vpt->current->type == pip)
		{
			if (!while_part_if_1(va))
				return (0);
		}
		else if (va->vpt->current->type == red)
		{
			va->vpt->result = while_part_if_2(va);
			if (va->vpt->result == 0)
				return 0;
			else if (va->vpt->result == 2) 
				return va->vpt->result;
			else if (va->vpt->result == 10)  // Heredoc was interrupted
            {
                // Clean up and return to prompt
                while_part_print_err(va);
                return 10;
            }
		}
		else if (va->vpt->current->type == text)
			add_argument(va->vpt->current_cmd, va->vpt->current->value, va);
		if (va->vpt->current)
			va->vpt->current = va->vpt->current->next;
	}
	while_part_print_err(va);
	if (va->vpt->syn_err)
		return (0);
	return (1);
}