#include "he.h"

int g_signal_pid;

int init_heredoc_buffer(t_sp_var *va)
{
	va->hrv->capacity = 10;
	va->hrv->bib = mmallocc(sizeof(char *) * va->hrv->capacity, &va->allocs, P_GARBAGE);
	if (!va->hrv->bib)
		return errno;
	return 0;
}

int read_heredoc_line(t_sp_var *va)
{
	g_signal_pid = 2;
	if (g_signal_pid == -1)
		return -2;
	
	write(1, "> ", 2);
	va->hrv->n = heredoc_readline(&va->hrv->line, va);
	
	if (!va->hrv->line || va->hrv->n == -2 || va->hrv->n == 0)
		return -2;
		
	if (va->hrv->n > 0 && va->hrv->line[va->hrv->n - 1] == '\n')
		va->hrv->line[va->hrv->n-1] = '\0';
		
	if (ft_strcmp(va->hrv->line, va->hrv->file_token->value) == 0)
		return 1;
	
	return 0;
}

void expand_heredoc_line(t_sp_var *va)
{
	if (va->hrv->i == 1)
	{
		va->var->state = UNQUOTED;
		va->hrv->tmp = expand_env_vars(va->hrv->line, va);
		va->hrv->line = va->hrv->tmp;
	}
}

/**
 * Resize heredoc buffer if needed
 */
int resize_heredoc_buffer(t_sp_var *va)
{
	if (va->hrv->in >= va->hrv->capacity - 1)
	{
		va->hrv->capacity *= 2;
		va->hrv->new_bib = mmallocc(sizeof(char *) * va->hrv->capacity, &va->allocs, P_GARBAGE);
		if (!va->hrv->new_bib)
			return errno;
			
		va->hrv->j = -1;
		while (++va->hrv->j < va->hrv->in)
			va->hrv->new_bib[va->hrv->j] = va->hrv->bib[va->hrv->j];
			
		va->hrv->bib = va->hrv->new_bib;
	}
	return 0;
}

/**
 * Processes the heredoc content into a temporary file
 */
int heredoc_to_temp_file(t_sp_var *va)
{
	g_signal_pid = 0;
	va->hrv->bib[va->hrv->in] = NULL;

	va->hrv->fd = open("t_m_p_f_i_l_e", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (va->hrv->fd < 0)
		return errno;
		
	va->hrv->j = -1;
	while (++va->hrv->j < va->hrv->in)
	{
		write(va->hrv->fd, va->hrv->bib[va->hrv->j], ft_strlen(va->hrv->bib[va->hrv->j]));
		write(va->hrv->fd, "\n", 1);
	}
	
	close(va->hrv->fd);
	va->hrv->redir->fd = open("t_m_p_f_i_l_e", O_RDONLY);
	return 0;
}