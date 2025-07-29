#include "pipex_bonus.h"

static int	get_next_input(char *line, size_t limiter_len,
		char *limiter, int fd[])
{
	size_t	line_len;

	line_len = ft_strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
		line[line_len - 1] = '\0';
	if (ft_strncmp(line, limiter, limiter_len) == 0
		&& ft_strlen(line) == limiter_len)
	{
		free(line);
		return (0);
	}
	line[line_len - 1] = '\n';
	write(fd[1], line, line_len);
	free(line);
	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	return (1);
}

static void	exec_heredoc(char *limiter, int fd[])
{
	char	*line;
	size_t	limiter_len;

	close(fd[0]);
	limiter_len = ft_strlen(limiter);
	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (get_next_input(line, limiter_len, limiter, fd) == 0)
			break ;
	}
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

void	here_doc(char *limiter)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		error(ERR_PIPE);
	pid = fork();
	if (pid == -1)
		error(ERR_PRC);
	if (pid == 0)
		exec_heredoc(limiter, fd);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(pid, &status, 0);
	}
}
