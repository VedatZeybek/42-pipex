#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include "../pipex.h"

int		validate_args_bonus(int argc, char **argv);
void	here_doc(char *limiter);
char	*get_next_line(int fd);

#endif