#include "pipex.h"

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

void	first_child(int	*pipe_fd, char **argv, char **env)
{
	pid_t	pid;
	int		infile;
	char	**cmd1_args;
	char	*cmd_path;

	cmd1_args = ft_split(argv[2], ' ');
	infile = open_file(argv[1], 0);
	pid = fork();
	if (pid < 0)
		error(ERR_PRC);
	if (pid == 0)
	{
		dup2(infile, STDIN_FILENO);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close_files(infile, pipe_fd[1], pipe_fd[0]);
		cmd_path = get_cmd_path(cmd1_args[0], env);
		if (!cmd_path)
		{
			free_splitted(cmd1_args);
			error(ERR_CMD);
		}
		if (execve(cmd_path, cmd1_args, env) == -1)
			error(ERR_EXCVE);
	}
	free_splitted(cmd1_args);
}

void	second_child(int *pipe_fd, char **argv, char **env)
{
	pid_t	pid;
	int		outfile;
	char	**cmd2_args;
	char	*cmd_path;

	cmd2_args = ft_split(argv[3], ' ');
	outfile = open_file(argv[4], 1);
	pid = fork();
	if (pid < 0)
		error(ERR_PRC);
	if (pid == 0)
	{
		dup2(pipe_fd[0], STDIN_FILENO);
		dup2(outfile, STDOUT_FILENO);
		close_files(outfile, pipe_fd[0], pipe_fd[1]);
		cmd_path = get_cmd_path(cmd2_args[0], env);
		if (!cmd_path)
		{
			free_splitted(cmd2_args);
			error(ERR_CMD);
		}
		if (execve(cmd_path, cmd2_args, env) == -1)
			error(ERR_EXCVE);
	}
	free_splitted(cmd2_args);
}

int	main(int argc, char **argv, char **env)
{
	int	pipe_fd[2];

	if (!validate_args(argc, argv))
		error(ERR_INPUT);
	if (pipe(pipe_fd) == -1)
		error(ERR_PIPE);
	first_child(pipe_fd, argv, env);
	second_child(pipe_fd, argv, env);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	wait(NULL);
	wait(NULL);
}
