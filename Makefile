NAME		= pipex

SRC			= ./mandatory/pipex.c ./mandatory/get_path.c ./mandatory/error.c

BONUS_SRC	= ./bonus/pipex_bonus.c ./bonus/utils_bonus.c  ./bonus/child_bonus.c \
			  ./bonus/get_next_line_bonus.c ./bonus/get_path_bonus.c ./bonus/error_bonus.c  

OBJ			= $(SRC:.c=.o)
BONUS_OBJ	= $(BONUS_SRC:.c=.o)

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
LIBFT		= ./libft/libft.a
LIBFT_DIR	= ./libft

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

bonus: $(BONUS_OBJ) $(LIBFT)
	@echo "Linking bonus into $(NAME)..."
	@$(CC) $(CFLAGS) $(BONUS_OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	@echo "Libft files compiling..."
	@$(MAKE) -s -C $(LIBFT_DIR) all

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ) $(BONUS_OBJ)
	@$(MAKE) -s -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re
