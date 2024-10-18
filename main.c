#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

#define TRUE 1
#define FALSE 0
#define PIPE '|'

enum token_type
{
	COMMAND
};

typedef struct s_command
{
	char	*value;
	char	**args;
}	t_command;

typedef struct s_token
{
	enum token_type	type;
	char			*value;
}	t_token;

char	*strfind(char *str, const char *set)
{
	char	*set_copy;

	set_copy = (char *)set;
	while (*str)
	{
		while(*set_copy)
		{
			if (*str == *set_copy)
				return str;
			else
				set_copy++;
		}
		str++;
		set_copy = (char *)set;
	}
	return str;
}

void	parse_command(char *command_string)
{
	t_command	command;
	char		*value;

	// Avoid whitespaces
	while (*command_string == ' ')
		command_string++;
	// First string separated by whitespaces or special characters is the command
	value = strfind(command_string, " <>|$\'\"\n\t\r\f");
	command.value = calloc(value - command_string + 1, sizeof(char));
	ft_strncpy(command.value, command_string, value - command_string);
	// We have found the command, now we can search for args, redirections ecc...
	
	printf("Command: %s\n", command.value);
	while (*value)
	{
		value = strfind(value,"<>|$\'\"");
		if (*value == '<')
			printf("Found <\n");
		else if (*value == '>')
			printf("Found >\n");
		value++;
	}
}

void	parse_input(char *input)
{
	char **command_strings;

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
