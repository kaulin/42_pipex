/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/20 13:23:27 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_n_mark(int *fd, t_piper **piper)
{
	if (*fd == -1)
		return ;
	if (close(*fd))
		fail(piper);
	*fd = -1;
}

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
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

static int	do_cmd(t_piper **piper)
{
	char	**cmd;
	char	*cmd_path;
	int		i;

	i = 0;
	cmd = ft_split((*piper)->cmdv[(*piper)->cmd_i], " ");
	if (!cmd)
		return (1);
	cmd_path = find_cmd_path(cmd[0], (*piper)->paths);
	if (!cmd_path)
	{
		clean_array(cmd);
		return (1);
	}
	if (execve(cmd_path, cmd, (*piper)->envp) == -1)
	{
		clean_array(cmd);
		free(cmd_path);
		return (1);
	}
	return (0);
}

static void	process(t_piper **piper)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		fail(piper);
	pid = fork();
	if (pid == -1)
		fail(piper);
	if (pid == 0)
	{
		close_n_mark(&fd[0], piper);
		if ((*piper)->cmd_i < (*piper)->cmdc - 1)
		{
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				fail(piper);
		}
		close_n_mark(&fd[1], piper);
		if ((*piper)->cmd_i == (*piper)->cmdc - 1)
		{
			if (access((*piper)->outfile, W_OK) != 0)
				fail(piper);
			if (dup2((*piper)->out_fd, STDOUT_FILENO) == -1)
				fail(piper);
		}
		if (do_cmd(piper))
			fail(piper);
	}
	close_n_mark(&fd[1], piper);
	(*piper)->pids[(*piper)->cmd_i] = pid;
	if (dup2(fd[0], STDIN_FILENO) == -1)
		fail(piper);
	close_n_mark(&fd[0], piper);
}

int	pipex(int argc, char *argv[], char **envp)
{
	t_piper	*piper;
	int		status;
	int		i;

	i = 0;
	init_piper(&piper, argc, argv, envp);
	if (dup2(piper->in_fd, STDIN_FILENO) == -1)
		fail(&piper);
	close_n_mark(&(piper->in_fd), &piper);
	while (piper->cmd_i < piper->cmdc)
	{
		process(&piper);
		piper->cmd_i++;
	}
	while (i < piper->cmdc)
	{
		if (waitpid(piper->pids[i], &status, 0) == -1)
			fail(&piper);
		dprintf(2, "Process %d id was %d and it exited with status %d\n", i, piper->pids[i], status);
		i++;
	}
	clean_piper(&piper);
	return (0);
}
