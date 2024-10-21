#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

#define TRUE 1
#define FALSE 0
#define BOOL char
#define PIPE '|'

enum token_type
{
	COMMAND
};

typedef struct s_command
{
	char	*value;
	char	**args;
	BOOL	right;
	BOOL	left;
}	t_command;

typedef struct s_token
{
	char			*value;
	enum token_type	type;
}	t_token;

void	parse_command(char *command_string)
{
	while (*command_string)
	{
		// Left redirection 
		if (ft_isalpha(*command_string))
		{
			printf("Found something\n");
			while (ft_isalpha(*(++command_string)) && *command_string) ;
		}
		else if (ft_isspace(*command_string))
			while (ft_isspace(*(++command_string))) ;
		else if (*command_string == '<')
		{
			if (*(command_string + 1) && *(command_string + 1) == '<')
			{
				command_string++;
				printf("Found <<\n");
			}
			else
				printf("Found <\n");
			command_string++;
		}
		// Right redirection
		else if (*command_string == '>')
		{
			if (*(command_string + 1) && *(command_string + 1) == '>')
			{
				command_string++;
				printf("Found >>\n");
			}
			else
				printf("Found >\n");
			command_string++;
		}
		// Strings
		else if (*command_string == '\"')
		{
			printf("Found String\n");
			while (*(++command_string) != '\"' && *command_string);
		}
		else if (*command_string == '\'')
		{
			printf("Found String\n");
			while (*(++command_string) != '\'' && *command_string);
		}
		// Enviroment variables
		else if (*command_string == '$')
		{
			printf("Found enviroment variable\n");
			if (*(command_string + 1) == '{')
			{
				while (*(++command_string) != '}' && *command_string);
				command_string++;
			}
			else
				while (ft_isalpha(*(++command_string)) && *command_string);
		}
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
