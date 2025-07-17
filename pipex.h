#ifndef PIPEX_H
# define PIPEX_H

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
# include "./libft/libft.h"

void	error(char *msg);
void	free_splitted(char **str);
char	*get_cmd_path(char *cmd, char **envp);
int		validate_args(int argc, char **argv);
void	close_files(int fd0, int fd1, int fd2);
int		open_file(char *file_name, int no);

#endif