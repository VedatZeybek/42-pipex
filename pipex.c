/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzeybek <vzeybek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:08:06 by vzeybek           #+#    #+#             */
/*   Updated: 2025/07/02 11:16:12 by vzeybek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/wait.h"

char	*get_env(char **env)
{
	int		i;
	char	*str;

	i = 0;
	while (env[i])
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

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**cmd_splitted;
	char	**path_splitted;
	char	*cmd_path;

	cmd_splitted = ft_split(cmd, ' ');
	path_splitted = ft_split(get_env(env), ':');
	i = 0;
	while (path_splitted[i])
	{
		cmd_path = ft_strjoin(path_splitted[i], "/");
		cmd_path = ft_strjoin(cmd_path, cmd_splitted[0]);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		i++;
	}
	return (NULL);
}

int	child_process1(int *fd, int *pipefd, char **argv, char **env)
{
	pid_t	pid0;
	char	**str;

	str = ft_split(argv[2], ' ');
	pid0 = fork();
	if (pid0 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		close(fd[0]);
		close(fd[1]);
		execve(get_path(str[0], env), str, env);
	}
	return (pid0);
}

int	child_process2(int *fd, int *pipefd, char **argv, char **env)
{
	pid_t	pid1;
	char	**str;

	str = ft_split(argv[3], ' ');
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		close(fd[0]);
		close(fd[1]);
		execve(get_path(str[0], env), str, env);
	}
	return (pid1);
}

int	main(int argc, char **argv, char **env)
{
	int		fd[2];
	int		pipefd[2];
	pid_t	pid0;
	pid_t	pid1;

	if (argc != 5)
		return (1);
	fd[0] = open(argv[1], O_CREAT | O_RDWR, 0644);
	fd[1] = open(argv[4], O_CREAT | O_RDWR, 0644);
	pipe(pipefd);
	child_process1(fd, pipefd, argv, env);
	child_process2(fd, pipefd, argv, env);
	close(pipefd[1]);
	close(pipefd[0]);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid0, NULL, 0);
	waitpid(pid1, NULL, 0);
}
