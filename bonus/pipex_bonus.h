#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define ERR_INPUT "Invalid number of arguments."
# define ERR_FILE "Invalid File."
# define ERR_PATH "Path Not Found."
# define ERR_CMD "Command Not Found."
# define ERR_PRC "Process Failded."
# define ERR_PIPE "Pipe Creation Failded."
# define ERR_EXCVE "Execution Failded."
# define ERR_CLS "Close File Failded."

# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include "../libft/libft.h"

int		validate_args_bonus(int argc, char **argv);
int		validate_args(int argc, char **argv);
int		open_file(char *file_name, int no);
char	*get_next_line(int fd);
char	*get_cmd_path(char *cmd, char **envp);
void	free_splitted(char **str);
void	error(char *msg);
void	here_doc(char *limiter);
void	close_files(int fd0, int fd1, int fd2);
void	child_cmd(char *argv, char **env);

#endif