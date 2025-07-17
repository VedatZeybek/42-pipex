NAME = pipex

SRC = pipex.c get_path.c error.c

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT =	./libft/libft.a


%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	@echo "Libft files compiling..."
	@$(MAKE) -s -C ./libft all

clean:
	@echo "Object files cleaning..."
	@rm -rf $(OBJ)
	@$(MAKE) -s -C ./libft fclean

fclean: clean
	@echo "All files cleaning..."
	@rm -rf $(NAME)
	@$(MAKE) -s -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re