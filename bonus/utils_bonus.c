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

	close(fd[0]);
	write(1, "heredoc> ", 9);
	line = get_next_line(0);
	while (line != NULL)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
	}
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

void	here_doc(char *limiter)
{
	int		fd[2];
	pid_t	pid;

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
		wait(NULL);
	}
}
