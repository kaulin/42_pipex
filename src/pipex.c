/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/23 15:49:10 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

static int	do_cmd(t_piper **piper)
{
	char	**cmd;
	char	*cmd_path;
	int		i;

	i = 0;
	//TODO: Handle single quotations
	cmd = split_esc((*piper)->cmdv[(*piper)->cmd_i], " ", 39);
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
	if (execve(cmd_path, cmd, (*piper)->envp) == -1)
	{
		clean_array(cmd);
		free(cmd_path);
		return (1);
	}
	return (0);
}

static void child(int *fd, t_piper **piper)
{
	if ((*piper)->cmd_i == 0)
	{
		(*piper)->in_fd = open((*piper)->infile, O_RDONLY);
		if ((*piper)->in_fd == -1 || dup2((*piper)->in_fd, STDIN_FILENO) == -1)
			fail("", piper);
		close((*piper)->in_fd);
	}
	close(fd[0]);
	if ((*piper)->cmd_i < (*piper)->cmdc - 1 \
		&& dup2(fd[1], STDOUT_FILENO) == -1)
		fail("", piper);
	close(fd[1]);
	if ((*piper)->cmd_i == (*piper)->cmdc - 1)
	{
		(*piper)->out_fd = \
			open((*piper)->outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
		if ((*piper)->out_fd == -1 \
			|| dup2((*piper)->out_fd, STDOUT_FILENO) == -1)
			fail("", piper);
		close((*piper)->out_fd);
	}
	if (do_cmd(piper))
		fail((*piper)->cmdv[(*piper)->cmd_i], piper);
}

static void	parent(t_piper **piper)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		fail("Pipe failed", piper);
	pid = fork();
	if (pid == -1)
		fail("Fork failed", piper);
	if (pid == 0)
		child(fd, piper);
	close(fd[1]);
	(*piper)->pids[(*piper)->cmd_i] = pid;
	if (dup2(fd[0], STDIN_FILENO) == -1)
		fail("", piper);
	close(fd[0]);
}

int	pipex(int argc, char *argv[], char **envp)
{
	t_piper	*piper;
	int		i;
	int		status;
	int		es;

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
		es = 0;
		if (waitpid(piper->pids[i], &status, 0) == -1)
			fail("Waitpid failed", &piper);
		if (WIFEXITED(status))
			es = WEXITSTATUS(status);
		dprintf(2, "Process %d id was %d, it exited with status %d - es %d\n", i, piper->pids[i], status, es);
		i++;
	}
	clean_piper(&piper);
	dprintf(2, "Final exit code: %d\n", status);
	exit(es);
}
