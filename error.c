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
