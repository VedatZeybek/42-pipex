#include "./libft/libft.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdio.h"
#include "sys/wait.h"



char	*get_env(char **env)
{
	int 	i;
	char *str;

	i = 0;
	while(env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			{
				str = ft_substr(env[i], 5, ft_strlen(env[i]));
				return (str);
			}
		i++;
	}
	return (NULL);
}

void get_path(char *cmd, char **env)
{
	int		i;
	char *exec;

	
}



int main(int argc, char **argv, char **env)
{
	pid_t pid0;
	pid_t pid1;
	int	status;
	int fd_in = open("input.txt", O_CREAT | O_RDWR, 0644);
	int fd_out = open("output.txt", O_CREAT | O_RDWR, 0644);
	int pipefd[2];

	pipe(pipefd);
	pid0 = fork();
	if (pid0 == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		char *str2[] = {"grep", "pipex.c", NULL};
		close(pipefd[0]);
		close(pipefd[1]);
		close(fd_in);
		close(fd_out);
		execve("/usr/bin/grep", str2, env);
	}

	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		char *str1[] = {"cat", NULL};
		close(pipefd[1]);
		close(pipefd[0]);
		close(fd_in);
		close(fd_out);
		execve("/usr/bin/cat", str1, env);

	}
	int i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	close(pipefd[1]);
	close(pipefd[0]);
	close(fd_in);
	close(fd_out);
	waitpid(pid0, &status, 0);
	waitpid(pid1, &status, 0);
}