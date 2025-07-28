#include "pipex_bonus.h"

int	validate_args_bonus(int argc, char **argv)
{
	int	i;

	if (argc < 5)
		return (0);
	if (!argv[1] || !argv[2] || !argv[argc - 1])
		return (0);
	if (argv[1][0] == '\0' || argv[argc - 1][0] == '\0')
		return (0);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6 || !argv[2] || argv[2][0] == '\0')
			return (0);
		i = 3;
	}
	else
		i = 2;
	while (i < argc - 1)
	{
		if (!argv[i] || argv[i][0] == '\0')
			return (0);
		i++;
	}
	return (1);
}

static void	exec_heredoc(char *limiter, int fd[])
{
	char	*line;
	size_t	limiter_len;
	size_t	line_len;

	close(fd[0]);
	limiter_len = ft_strlen(limiter);
	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		line_len = ft_strlen(line);
		if (line_len > 0 && line[line_len - 1] == '\n')
			line[line_len - 1] = '\0';
		if (ft_strncmp(line, limiter, limiter_len) == 0 && 
			ft_strlen(line) == limiter_len)
		{
			free(line);
			break ;
		}
		line[line_len - 1] = '\n';
		write(fd[1], line, line_len);
		free(line);
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
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
