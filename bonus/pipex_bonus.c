#include "pipex_bonus.h"

static void	execute_last_cmd(char *argv, char **env, int file_output)
{
	pid_t	pid;
	char	**args;
	char	*cmd_path;

	pid = fork();
	if (pid == -1)
		error(ERR_PRC);
	if (pid == 0)
	{
		dup2(file_output, STDOUT_FILENO);
		close(file_output);
		args = ft_split(argv, ' ');
		cmd_path = get_cmd_path(args[0], env);
		if (!cmd_path)
		{
			free_splitted(args);
			error(ERR_CMD);
		}
		if (execve(cmd_path, args, env) == -1)
			error(ERR_EXCVE);
	}
	else
	{
		close(file_output);
		waitpid(pid, NULL, 0);
	}
}

static void	first_file(int argc, char **argv, int *file_output, int *i)
{
	int	file_input;

	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		*i = 3;
		*file_output = open_file(argv[argc - 1], 1);
		here_doc(argv[2]);
	}
	else
	{
		*i = 2;
		*file_output = open_file(argv[argc - 1], 1);
		file_input = open_file(argv[1], 0);
		dup2(file_input, STDIN_FILENO);
		close(file_input);
	}
}

int	main(int argc, char **argv, char **env)
{
	int		i;
	int		file_output;

	if (argc < 5)
		error(ERR_INPUT);
	if (!validate_args_bonus(argc, argv))
		error(ERR_INPUT);
	first_file(argc, argv, &file_output, &i);
	while (i < argc - 2)
		child_cmd(argv[i++], env);
	execute_last_cmd(argv[argc - 2], env, file_output);
	return (0);
}
