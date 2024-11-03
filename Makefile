SRC = main.c

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)libft.a

FLAGS = -g #-Wall -Wextra -Werror

all: LIBFT
	gcc main.c $(FLAGS) -lreadline -lft -L$(LIBFT_DIR) -o minishell

LIBFT:
	@$(MAKE) bonus -C $(LIBFT_DIR)
