#include "pipex.h"

int	validate_args(int argc, char **argv)
{
	if (argc != 5)
		return (0);
	if (!argv[1] || !argv[2] || !argv[3] || !argv[4])
		return (0);
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		return (0);
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
