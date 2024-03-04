/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/04 12:36:34 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Looks for specific command in the paths of the environment variable.
*/
static char	*find_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

/*
Splits given command string to an array of actual command and command options. If the command contains a path, uses that instead of the environment variable. Calls execve with given path and command array and environment.
*/
static int	do_cmd(t_piper **piper)
{
	char	**cmd;
	char	*cmd_path;
	int		i;

	i = 0;
	cmd = split_quote((*piper)->cmdv[(*piper)->cmd_i], " ");
	if (!cmd)
		return (1);
	if (ft_strchr(cmd[0], '/'))
	{
		cmd_path = ft_strdup(cmd[0]);
		cmd[0] = ft_strdup(ft_strrchr(cmd[0], '/') + 1);
	}
	else
		cmd_path = find_cmd_path(cmd[0], (*piper)->paths);
	if (!cmd_path)
	{
		clean_array(cmd);
		return (1);
	}
	execve(cmd_path, cmd, (*piper)->envp);
	clean_array(cmd);
	free(cmd_path);
	return (1);
}

/*
Child process handles input and output redirection and calls do_cmd to actually execute execve.
*/
static void	child(int *fd, t_piper **piper)
{
	close(fd[0]);
	if ((*piper)->cmd_i < (*piper)->cmdc - 1 \
		&& dup2(fd[1], STDOUT_FILENO) == -1)
		fail(EXIT_FAILURE, "", piper);
	close(fd[1]);
	if (ft_strlen((*piper)->cmdv[(*piper)->cmd_i]) == 0)
		fail(127, (*piper)->cmdv[(*piper)->cmd_i], piper);
	if ((*piper)->cmd_i == 0)
	{
		(*piper)->in_fd = open((*piper)->infile, O_RDONLY);
		if ((*piper)->in_fd == -1 || dup2((*piper)->in_fd, STDIN_FILENO) == -1)
			fail(EXIT_FAILURE, (*piper)->infile, piper);
		close((*piper)->in_fd);
	}
	if ((*piper)->cmd_i == (*piper)->cmdc - 1)
	{
		(*piper)->out_fd = \
			open((*piper)->outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
		if ((*piper)->out_fd == -1 \
			|| dup2((*piper)->out_fd, STDOUT_FILENO) == -1)
			fail(EXIT_FAILURE, (*piper)->outfile, piper);
		close((*piper)->out_fd);
	}
	if (do_cmd(piper))
		fail(127, (*piper)->cmdv[(*piper)->cmd_i], piper);
}

/*
Creates a pipe, forks, stores child PID to struct and duplicates STDIN file 
descriptor to pipe's read end. Enssure's both write and read ends are closed, 
even if error's occur.
*/
static void	parent(t_piper **piper)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		fail(EXIT_FAILURE, "Pipe failed", piper);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		fail(EXIT_FAILURE, "Fork failed", piper);
	}
	if (pid == 0)
		child(fd, piper);
	close(fd[1]);
	(*piper)->pids[(*piper)->cmd_i] = pid;
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		close(fd[0]);
		fail(EXIT_FAILURE, "", piper);
	}
	close(fd[0]);
}

/*
Initialises piper struct with information from received aguments. 
Calls parent function for each command parameter. Waits for all 
children to exit, cleans the piper struct and exits with the last 
childs exit status.
*/
int	pipex(int argc, char *argv[], char **envp)
{
	t_piper	*piper;
	int		i;
	int		status;
	int		exit_status;

	i = 0;
	status = 0;
	init_piper(&piper, argc, argv, envp);
	while (piper->cmd_i < piper->cmdc)
	{
		parent(&piper);
		piper->cmd_i++;
	}
	while (i < piper->cmdc)
	{
		exit_status = 0;
		if (waitpid(piper->pids[i], &status, 0) == -1)
			fail(EXIT_FAILURE, "Waitpid failed", &piper);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		i++;
	}
	clean_piper(&piper);
	exit(exit_status);
}
