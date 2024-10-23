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
#define MAX_ARGS 100
#define MAX_COMMANDS 100
#define MAX_TOKEN 100
#define MAX_STRING 100

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

typedef struct s_command
{
	unsigned int	arg_count;
	char			value[MAX_STRING];
	char			args[MAX_ARGS][MAX_STRING];
	char			left_arg[MAX_STRING];
	char			right_arg[MAX_STRING];
	BOOL			right;
	BOOL			left;
	BOOL			double_right;
	BOOL			double_left;
}	t_command;

void	print_command(t_command command)
{
	printf("Value: %s\n", command.value);
	if (command.right)
		printf("Command has right redirection!\n");
	if (command.left)
		printf("Command has left redirection!\n");
	if (command.double_right)
		printf("Command has double right redirection!\n");
	if (command.double_left)
		printf("Command has double left redirection!\n");
}

typedef struct s_token
{
	unsigned int	len;
	char			*value;
	enum token_type	type;
}	t_token;

t_command	new_command()
{
	t_command command;

	command.arg_count = 0;
	command.right = FALSE;
	command.left = FALSE;
	command.double_right = FALSE;
	command.double_left = FALSE;
	return (command);
}



t_command	parse_command(char *command_string)
{
	char			*beginning;
	t_command		command;
	t_token			*token;

	command = new_command();
	while (*command_string)
	{
		beginning = NULL;
		token = (t_token *)calloc(1, sizeof(t_token));
		while (ft_isspace(*command_string))
		{
			command_string++;
		}

		if (ft_isalpha(*command_string))
		{
			printf("Found something %s\n", command_string);
			beginning = command_string;
			while (ft_isalpha(*command_string) && *command_string)
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token :)
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
			printf("Token: %s\n", token->value);
		}
		else if (*command_string == '<')
		{
			if (*(command_string + 1) == '<')
			{
				printf("Found <<\n");
				command_string += 2;
			}
			else
			{
				printf("Found <\n");
				command_string += 1;
			}
		}
		free(token);
	}
	return (command);
}

void	parse_input(char *input)
{
	char		**command_strings;
	t_command	current;

	command_strings = ft_split(input, PIPE);
	while (*command_strings)
	{
		printf("Parsing command %s\n", *command_strings);
		current = parse_command(*command_strings++);
		print_command(current);
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
