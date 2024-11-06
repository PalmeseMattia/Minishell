#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "libft/libft.h"

#define TRUE 1
#define FALSE 0
#define BOOL char
#define PIPE '|'

enum token_type
{
	INVALID,
	COMMAND,
	ARG,
	LEFT,
	RIGHT,
	DOUBLE_LEFT,
	DOUBLE_RIGHT,
	LEFT_ARG,
	RIGHT_ARG,
};

typedef struct s_token
{
	unsigned int	len;
	char			*value;
	enum token_type	type;
}	t_token;

typedef struct s_lexer
{
	t_token			*tokens;
	unsigned int	num_tokens;
}	t_lexer;

BOOL	has_command(t_list *tokens)
{
	t_list	*node;

	if (!tokens)
		return (FALSE);
	node = tokens;
	while (node)
	{
		if (((t_token *)(node->content))->type == COMMAND)
			return (TRUE);
		node = node->next;
	}
	return (FALSE);
}

void	get_token_type(t_token *token, t_list *tokens)
{
	t_list	*last_node;
	t_token	*last_token;

	// Redirection cases
	if (token->len == 2 && ft_strncmp(token->value, "<<", 2) == 0) {
		printf("Token type is double left\n");
		token->type = DOUBLE_LEFT;
	}
	else if (token->len == 2 && ft_strncmp(token->value, ">>", 2) == 0) {
		printf("Token type is double right\n");
		token->type = DOUBLE_RIGHT;
	}
	else if (token->len == 1 && ft_strncmp(token->value, "<", 1) == 0) {
		printf("Token type is left\n");
		token->type = LEFT;
	}
	else if (token->len == 1 && ft_strncmp(token->value, ">", 1) == 0) {
		printf("Token type is right\n");
		token->type = RIGHT;
	}
	// TODO: remember to add other bash symbols
	else {
		last_node = ft_lstlast(tokens);
		// If this can be a command, we check if we already have a command
		if (last_node) {
			last_token = (t_token *)last_node->content;
			printf("Last token was %s of type %d\n", last_token->value, last_token->type);
		}
		// Create function to check if a command exists
		if (last_node && (last_token->type == DOUBLE_LEFT || last_token->type == LEFT))
		{
			printf("This is the left redirection argument\n");
			token->type = LEFT_ARG;
		}
		else if (last_node && (last_token->type == DOUBLE_RIGHT || last_token->type == RIGHT))
		{
			printf("This is the right redirection argument\n");
			token->type = RIGHT_ARG;
		}
		else if(last_node && (last_token->type == COMMAND || last_token->type == ARG))
		{
			printf("This is an argument\n");
			token->type = ARG;
		}
		else if (!last_node || !has_command(tokens))
		{
			printf("This is a command\n");
			token->type = COMMAND;
		}
	}
}

void	parse_command(char *command_string)
{
	char			*beginning;
	t_token			*token;
	t_list			*tokens;

	tokens = NULL;
	while (*command_string)
	{
		beginning = NULL;
		token = (t_token *)calloc(1, sizeof(t_token));
		while (ft_isspace(*command_string))
		{
			command_string++;
		}

		if (ft_isalpha(*command_string) || *command_string == '-')
		{
			beginning = command_string;
			while (ft_isalpha(*command_string) || *command_string == '-' && *command_string)
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
		}
		else if (*command_string == '<')
		{
			if (*(command_string + 1) == '<')
			{
				token -> len = 2;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  "<<", token->len);
				command_string += 2;
			}
			else
			{
				token -> len = 1;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  "<", token->len);
				command_string += 1;
			}
		}
		else if (*command_string == '>')
		{
			if (*(command_string + 1) == '>')
			{
				token -> len = 2;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  ">>", token->len);
				command_string += 2;
			}
			else
			{
				token -> len = 1;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  ">", token->len);
				command_string += 1;
			}
		}
		printf("Token: %s\n", token->value);
		get_token_type(token, tokens);
		ft_lstadd_back(&tokens, ft_lstnew(token));
		printf("\n");
	}
}

void	parse_input(char *input)
{
	char		**command_strings;

	command_strings = ft_split(input, PIPE);
	while (*command_strings)
	{
		printf("Parsing command %s\n", *command_strings);
		parse_command(*command_strings++);
	}
}

int main()
{
	const char	*prompt;
	char		*input;

	prompt = "minishell > ";
	while (1)
	{
		input = readline(prompt);
		parse_input(input);

		free(input);
	}
	return (0);
}
