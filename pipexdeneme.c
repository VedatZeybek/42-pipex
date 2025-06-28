#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "./libft/libft.h"
#include <sys/wait.h>

// int main(int argc, char **argv, char **envp)
// {

// 	if (argc != 5)
// 		return (1);

// 	int fd_in = open(argv[1], O_RDONLY);

// 	int fd_out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);

// 	char **cmd1_args = split(argv[2], ' ');
// 	char **cmd2_args = split(argv[3], ' ');
// 	char *cmd1_path = get_cmd_path(cmd1_args[0], envp);
// 	char *cmd2_path = get_cmd_path(cmd2_args[0], envp);

// 	int pipefd[2];
// 	pipe(pipefd);

// 	pid_t pid1 = fork();
// 	if (pid1 == 0)
// 	{
// 		dup2(fd_in, STDIN_FILENO);
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		close(pipefd[0]);
// 		execve(cmd1_path, cmd1_args, envp);
// 	}

// 	pid_t pid2 = fork();
// 	if (pid2 == 0)
// 	{
// 		dup2(pipefd[0], STDIN_FILENO);
// 		dup2(fd_out, STDOUT_FILENO);
// 		close(pipefd[1]);
// 		execve(cmd2_path, cmd2_args, envp);
// 	}
// 	waitpid(pid1, NULL, 0);
// 	waitpid(pid2, NULL, 0);
// }

// char *get_cmd_path(char *cmd, char **envp)
// {
// 	int i = 0;
// 	char *path = NULL;
// 	char **paths = NULL;
// 	char *full_path = NULL;

// 	while (envp[i] && strncmp(envp[i], "PATH=", 5) != 0)
// 		i++;
//     if (!envp[i])
//         return NULL;
    
//     path = envp[i] + 5;
//     paths = ft_split(path, ':');

//     i = 0;
//     while (paths[i])
//     {
//         full_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
//         if (access(full_path, X_OK) == 0)
//         {
//             return full_path;
//         }
//         free(full_path);
//         i++;
//     }
//     return NULL;
// }
extern char **environ;


int main()
{
	int fd0 = open("input.txt", O_RDONLY, 0644);
	int fd1 = open("output.txt", O_CREAT | O_RDWR, 0644);

	int pipefd[2];
	pid_t pid1, pid2;
	pipe(pipefd);

	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fd0, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		close(fd0);
		close(fd1);

		char *args[] = {"cat", NULL};
		execve("/usr/bin/cat", args, environ);
	}

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		dup2(fd1, STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
        close(fd0);
        close(fd1);

		char *args[] = {"grep", "hello", NULL};
		execve("/usr/bin/grep", args, environ);
	}

	close(pipefd[0]);
    close(pipefd[1]);
    close(fd0);
    close(fd1);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}