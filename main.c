#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type
{
	COMMAND,
	PIPE,
	ARG,
	REDIR_OUT,
	REDIR_IN,
	REDIR_OUT_A,
	REDIR_IN_D,
	DELIMIT,
	FILENAME,
	STRING
}	token_type;

typedef struct s_token
{
	char		*value;
	token_type	type;
}	t_token;

int main()
{
	const char	*prompt;
	char		*input;

	prompt = "minishell > ";
	while (1)
	{
		input = readline(prompt);
		printf("%s\n", input);
		free(input);
	}
	return (0);
}
