/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/04/02 16:08:56 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Function checks if the current command is the first or last to be executed. 
For the first command, an input file is opened, duplicated to STDIN and then 
closed. For the last, an output file is opened (also created with correct 
permissions if necessary), duplicated to STDOUT and then closed. 
*/
static void	open_dub_close(t_piper **piper)
{
	if ((*piper)->cmd_i == 0)
	{
		if (!(*piper)->here_doc)
			(*piper)->in_fd = open((*piper)->infile, O_RDONLY);
		if ((*piper)->in_fd == -1 || dup2((*piper)->in_fd, STDIN_FILENO) == -1)
			fail(1, (*piper)->infile, piper);
		close((*piper)->in_fd);
	}
	if ((*piper)->cmd_i == (*piper)->cmdc - 1)
	{
		if ((*piper)->here_doc)
		{
			(*piper)->out_fd = \
				open((*piper)->outfile, O_APPEND | O_CREAT | O_RDWR, 0644);
		}
		else
		{
			(*piper)->out_fd = \
				open((*piper)->outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
		}
		if ((*piper)->out_fd == -1 \
			|| dup2((*piper)->out_fd, STDOUT_FILENO) == -1)
			fail(1, (*piper)->outfile, piper);
		close((*piper)->out_fd);
	}
}

/*
Child process handles input and output redirection and calls do_cmd to actually 
execute execve.
*/
static void	child(int *fd, t_piper **piper)
{
	int	err_code;

	close(fd[0]);
	if ((*piper)->cmd_i < (*piper)->cmdc - 1 \
		&& dup2(fd[1], STDOUT_FILENO) == -1)
		fail(1, "Dup2 failed", piper);
	close(fd[1]);
	if (*((*piper)->cmdv[(*piper)->cmd_i]) == 0)
		fail(127, (*piper)->cmdv[(*piper)->cmd_i], piper);
	open_dub_close(piper);
	err_code = do_cmd(piper);
	if (err_code == 1)
		fail(err_code, "Memory allocation error", piper);
	if ((*piper)->cmd_err)
		fail(err_code, (*piper)->cmd_err, piper);
	fail(err_code, (*piper)->cmdv[(*piper)->cmd_i], piper);
}

/*
Creates a pipe, forks, stores child PID to struct and duplicates STDIN file 
descriptor to pipe's read end. Ensure's both write and read ends are closed, 
even if errors occur.
*/
static void	parent(t_piper **piper)
{
	int		fd[2];

	if (pipe(fd) == -1)
		fail(666, "Pipe failed", piper);
	(*piper)->pids[(*piper)->cmd_i] = fork();
	if ((*piper)->pids[(*piper)->cmd_i] == -1)
	{
		close(fd[0]);
		close(fd[1]);
		fail(666, "Fork failed", piper);
	}
	if ((*piper)->pids[(*piper)->cmd_i] == 0)
		child(fd, piper);
	(*piper)->cmd_i++;
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		close(fd[0]);
		fail(666, "Dup2 failed", piper);
	}
	close(fd[0]);
}

/*
Checks number of arguments, then initialises piper struct with information 
from received aguments. Calls parent function for each command parameter. 
Waits for all children to exit, cleans the piper struct and exits with the 
last child's exit status.
*/
int	main(int argc, char *argv[], char **envp)
{
	t_piper	*piper;
	int		i;
	int		status;
	int		exit_status;

	if (argc < 5)
	{
		ft_putstr_fd("Usage: <input file> <cmd 1> <cmd 2> <output file>\n", 2);
		exit(1);
	}
	i = 0;
	status = 0;
	init_piper(&piper, argc, argv, envp);
	while (piper->cmd_i < piper->cmdc)
		parent(&piper);
	while (i < piper->cmdc)
	{
		exit_status = 0;
		if (waitpid(piper->pids[i++], &status, 0) == -1)
			fail(1, "Waitpid failed", &piper);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	clean_piper(&piper);
	exit(exit_status);
}
