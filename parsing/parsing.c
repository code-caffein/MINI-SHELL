#include "he.h"




t_cmd *parse(char *line)
{
	t_token *tokens;
	t_cmd *commands;

	tokens = tokenize_input(line);
	if (!tokens)
		return (NULL);
	commands = parse_tokens(tokens);
	free_token_list(&tokens);
	return (commands);
}