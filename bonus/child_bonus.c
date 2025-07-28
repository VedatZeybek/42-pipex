#include "pipex_bonus.h"

static void	execute_child(char *argv, char **env, int fd[]) 
{
	char	**args;
	char	*cmd_path;

	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
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

static void	handle_parent(int fd[], pid_t pid)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(pid, NULL, 0);
}

void	child_cmd(char *argv, char **env)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error(ERR_PIPE);
	pid = fork();
	if (pid == -1)
		error(ERR_PRC);
	if (pid == 0)
		execute_child(argv, env, fd);
	else
		handle_parent(fd, pid);
}
