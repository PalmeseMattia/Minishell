SRC = main.c

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)libft.a

FLAGS = -Wall -Wextra -Werror -g

all: LIBFT
	gcc main.c $(FLAGS) -lreadline -lft -L$(LIBFT_DIR) -o minishell

LIBFT:
	@$(MAKE) -C $(LIBFT_DIR)
