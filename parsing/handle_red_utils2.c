#include "he.h"

int process_heredoc(t_sp_var *va)
{
	int result;
	
	result = init_heredoc_buffer(va);
	if (result != 0)
		return result;
	while (1)
	{
		result = read_heredoc_line(va);
		if (result != 0)
			break; // Either error or found delimiter
		
		expand_heredoc_line(va);
		
		result = resize_heredoc_buffer(va);
		if (result != 0)
			return result;
		
		va->hrv->bib[va->hrv->in++] = va->hrv->line;
	}
	
	if (result < 0)
		return result;
		
	return heredoc_to_temp_file(va);
}