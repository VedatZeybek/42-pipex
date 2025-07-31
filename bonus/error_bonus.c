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
	if (ft_strncmp(argv[1], "here_doc", 8) == 0
		&& ft_strlen("here_doc") == ft_strlen(argv[1]))
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

void	free_splitted(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	error(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(EXIT_FAILURE);
}

int	open_file(char *file_name, int no)
{
	int	fd;

	if (no == 0)
		fd = open(file_name, O_RDONLY, 0644);
	else if (no == 1)
		fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (no == 2)
		fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		error(ERR_FILE);
	return (fd);
}

void	close_files(int fd0, int fd1, int fd2)
{
	if (fd0 >= 0 && close(fd0) == -1)
		error(ERR_CLS);
	if (fd1 >= 0 && close(fd1) == -1)
		error(ERR_CLS);
	if (fd2 >= 0 && close(fd2) == -1)
		error(ERR_CLS);
}
