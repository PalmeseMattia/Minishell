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

typedef struct s_command
{
	char	*command;
	char	**args;
	char	*left_arg;
	char	*right_arg;
	int		argc;
	BOOL	left_redir;
	BOOL	double_left_redir;
	BOOL	right_redir;
	BOOL	double_right_redir;
}	t_command;

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

	if (token->len == 2 && ft_strncmp(token->value, "<<", 2) == 0)		// DOUBLE LEFT REDIRECTION
		token->type = DOUBLE_LEFT;
	else if (token->len == 2 && ft_strncmp(token->value, ">>", 2) == 0)	// DOUBLE RIGHT REDIRECTION
		token->type = DOUBLE_RIGHT;
	else if (token->len == 1 && ft_strncmp(token->value, "<", 1) == 0)	// LEFT REDIRECTION
		token->type = LEFT;
	else if (token->len == 1 && ft_strncmp(token->value, ">", 1) == 0)	// RIGHT REDIRECTION
		token->type = RIGHT;
	// TODO: remember to add other bash symbols
	else {
		last_node = ft_lstlast(tokens);
		if (last_node)
			last_token = (t_token *)last_node->content;
		if (last_node && (last_token->type == DOUBLE_LEFT || last_token->type == LEFT))			// DOUBLE LEFT REDIRECTION ARGUMENT
			token->type = LEFT_ARG;
		else if (last_node && (last_token->type == DOUBLE_RIGHT || last_token->type == RIGHT))	// DOUBLE RIGHT REDIRECTION ARGUMENT
			token->type = RIGHT_ARG;
		else if (!last_node || !has_command(tokens))											// COMMAND
			token->type = COMMAND;
		else
			token->type = ARG;
	}
	//printf("Token of type %d\n", token->type);
}

int	count_args(t_list *tokens)
{
	t_token	*current_tok;
	int	arguments;

	arguments = 0;
	while (tokens)
	{
		current_tok = (t_token *)tokens->content;
		if (current_tok->type == ARG)
			arguments++;
		tokens = tokens->next;
	}
	return (arguments);
}

//TODO: add arguments, redirections and so on
t_command	*create_command(t_list *tokens)
{
	t_list		*current_node;
	t_token		*current_tok;
	t_command	*command;
	int			token_size;

	current_node = tokens;
	command = (t_command *)calloc(1, sizeof(t_command));
	command->args = (char **)calloc(count_args(tokens) + 1, sizeof(char *));
	command->args[count_args(tokens)] = NULL;

	while (current_node)
	{
		current_tok = (t_token *)current_node->content;
		if (current_tok->type == COMMAND)
		{
			token_size = ft_strlen(current_tok -> value);
			command->command = (char *)calloc(token_size + 1, sizeof(char));
			ft_strncpy(command->command, current_tok->value, token_size + 1);
			//printf("Found Command: %s\n", command->command);
		}
		else if (current_tok->type == LEFT_ARG)
		{
			token_size = ft_strlen(current_tok -> value);
			command->left_arg = (char *)calloc(token_size + 1, sizeof(char));
			ft_strncpy(command->left_arg, current_tok->value, token_size + 1);
			//printf("Found left redirection argument %s\n", command->left_arg);
		}
		else if (current_tok->type == RIGHT_ARG)
		{
			token_size = ft_strlen(current_tok -> value);
			command->right_arg = (char *)calloc(token_size + 1, sizeof(char));
			ft_strncpy(command->right_arg, current_tok->value, token_size + 1);
			//printf("Found right redirection argument %s\n", command->right_arg);
		}
		else if (current_tok->type == LEFT)
		{
			command->double_left_redir = FALSE;
			command->left_redir = TRUE;
			//printf("Found left redirection!\n");
		}
		else if (current_tok->type == RIGHT)
		{
			command->double_right_redir = FALSE;
			command->right_redir = TRUE;
			//printf("Found right redirection!\n");
		}
		else if (current_tok->type == DOUBLE_LEFT)
		{
			command->double_left_redir = TRUE;
			command->left_redir = FALSE;
			//printf("Found double left redirection!\n");
		}
		else if (current_tok->type == DOUBLE_RIGHT)
		{
			command->double_right_redir = TRUE;
			command->right_redir = FALSE;
			//printf("Found double right redirection!\n");
		}
		else if (current_tok->type == ARG)
		{
			//printf("Found argument: %s\n", current_tok->value);
			command->args[command->argc] = calloc(current_tok->len + 1, sizeof(char));
			ft_strncpy(command->args[command->argc++], current_tok->value, current_tok->len); 
		}
		current_node = current_node->next;
	}
	return (command);
}

BOOL	is_valid_arg_char(char c)
{
	return (!ft_isspace(c) && !ft_strchr("|<>\"\'$", c));
}

void	expand(char **env_var)
{
	char *expanded;

	if (!env_var || !*env_var)
		return ;
	expanded = getenv(*env_var);
	if (!expanded)
	{
		*env_var = (char *)calloc(1, sizeof(char));
		*env_var[0] = '\0';
	}
	else
	{
		*env_var = (char *)calloc(ft_strlen(expanded), sizeof(char));
		ft_strncpy(*env_var, expanded, ft_strlen(expanded));
	}
}

t_command	*parse_command(char *command_string)
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
		if (*command_string == 0)
			break ;

		if (*command_string == '<')
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
		else if (*command_string == '"')
		{
			beginning = ++command_string;
			while (*command_string && *command_string != '"')
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
			command_string++;
		}
		else if (*command_string == '$')
		{
			//printf("Found env variable\n");
			beginning = ++command_string;
			while (*command_string && is_valid_arg_char(*command_string))
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
			expand(&token->value);
		}
		else if (*command_string)
		{
			beginning = command_string;
			while ((is_valid_arg_char(*command_string)) && *command_string)
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
		}
		//printf("Token: %s\n", token->value);
		get_token_type(token, tokens);
		ft_lstadd_back(&tokens, ft_lstnew(token));
	}
	//printf("Now we have the tokens, lets create a command to execute\n");
	return (create_command(tokens));
}

void	print_command(t_command *command)
{
		printf("Command: %s\n", command->command);
		printf("Arguments: ");
		for (int i = 0; command->args[i]; i++)
			printf("%s ", command->args[i]);
		printf("\n");
		if (command->left_redir)
		{
			printf("Left redirection: %s\n", command->left_arg);
		}
		else if (command->double_left_redir)
		{
			printf("Double Left redirection: %s\n", command->left_arg);
		}
		if (command->right_redir)
		{
			printf("Right redirection: %s\n", command->right_arg);
		}
		else if (command->double_right_redir)
		{
			printf("Double Right redirection: %s\n", command->right_arg);
		}
}


void	parse_input(char *input)
{
	char		**command_strings;
	t_command	*command;

	command_strings = ft_split(input, PIPE);
	while (*command_strings)
	{
		printf("Parsing command %s\n", *command_strings);
		command = parse_command(*command_strings++);
		print_command(command);
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
