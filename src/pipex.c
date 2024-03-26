/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/26 12:28:46 by jajuntti         ###   ########.fr       */
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
		fail(666, "Pipe failed", piper);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		fail(666, "Fork failed", piper);
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
Parses paths from environment variable.
*/
static void	parse_paths(char ***paths, char **envp)
{
	int		i;
	char	*joined_path;

	i = 0;
	if (!*envp)
		return ;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i] || !envp[i][5])
		return ;
	*paths = ft_split(&(envp[i][5]), ":");
	if (!(*paths))
		return ;
	i = 0;
	while ((*paths)[i])
	{
		joined_path = ft_strjoin((*paths)[i], "/");
		if (!joined_path)
		{
			clean_array(*paths);
			return ;
		}
		free((*paths)[i]);
		(*paths)[i++] = joined_path;
	}
}

/*
Initialises piper struct.
*/
static void	init_piper(t_piper **ppiper, int argc, char *argv[], char **envp)
{
	t_piper	*piper;

	piper = malloc(sizeof(t_piper));
	if (!piper)
		fail(EXIT_FAILURE, "Memory allocation error", &piper);
	piper->infile = argv[1];
	piper->outfile = argv[argc - 1];
	piper->paths = NULL;
	piper->pids = NULL;
	piper->in_fd = -1;
	piper->out_fd = -1;
	piper->cmd_i = 0;
	piper->cmdc = argc - 3;
	piper->cmdv = &(argv[2]);
	piper->envp = envp;
	piper->cmd_err = NULL;
	parse_paths(&piper->paths, envp);
	piper->pids = malloc(sizeof(pid_t) * piper->cmdc);
	if (!piper->pids)
		fail(EXIT_FAILURE, "Memory allocation error", &piper);
	*ppiper = piper;
}

/*
Initialises piper struct with information from received aguments. 
Calls parent function for each command parameter. Waits for all 
children to exit, cleans the piper struct and exits with the last 
child's exit status.
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
