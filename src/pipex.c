/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/07 12:16:53 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
