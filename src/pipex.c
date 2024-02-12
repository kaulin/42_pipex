/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/12 14:06:40 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char *find_cmd_path(char *cmd, char **paths)
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

static void	do_cmd(t_piper **piper)
{
	char	**cmd;
	char	*cmd_path;
	int i;

	i = 0;
	cmd = ft_split((*piper)->cmdv[(*piper)->cmd_i], " ");
	if (!cmd)
		fail(piper);
	cmd_path = find_cmd_path(cmd[0], (*piper)->paths);
	if (!cmd_path)
	{
		clean_array(cmd);
		free(cmd);
		fail(piper);
	}
	if (execve(cmd_path, cmd, (*piper)->envp) == -1)
	{
		clean_array(cmd);
		free(cmd);
		free(cmd_path);
		fail(piper);
	}
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
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			fail(piper);
		close(fd[1]);
		if ((*piper)->cmd_i == (*piper)->cmdc - 1 \
			&& dup2((*piper)->out_fd, STDOUT_FILENO) == -1)
			fail(piper);
		do_cmd(piper);
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		fail(piper);
	close(fd[0]);
	waitpid(pid, NULL, 0);
}
	
int	pipex(int argc, char *argv[], char **envp)
{
	t_piper	*piper;

	init_piper(&piper, argc, argv, envp);
	if (dup2(piper->in_fd, STDIN_FILENO) == -1)
			fail(&piper);
	while (piper->cmd_i < piper->cmdc)
	{
		process(&piper);
		piper->cmd_i++;
	}
	clean_piper(&piper);
	return (0);
}
