SRC = main.c

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)libft.a

all: LIBFT
	gcc main.c -lreadline -lft -L$(LIBFT_DIR) -o minishell

LIBFT:
	@$(MAKE) -C $(LIBFT_DIR)
