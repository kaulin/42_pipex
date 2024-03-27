/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 12:16:04 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/27 10:33:43 by jajuntti         ###   ########.fr       */
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
}

/*
Frees an array of srings and a string given as arguments and returns the given 
integer value.
*/
static int	clean_return(char **arr, char *str, int ret)
{
	if (arr)
		clean_array(arr);
	if (str)
		free(str);
	return (ret);
}

/*
Looks for specific command in the paths of the environment variable.
*/
static char	*find_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (!paths)
		return (NULL);
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

/*
Splits given command string to an array of actual command and command options 
If command not found from environment paths, tries to use the command itself as 
path. Calls execve with given path and command array and environment.
*/
static int	do_cmd(t_piper **piper)
{
	char	**cmd;
	char	*cmd_path;

	if (ft_strchr(" \t\v\n\r\f", (*piper)->cmdv[(*piper)->cmd_i][0]))
		return (127);
	cmd = split_quote((*piper)->cmdv[(*piper)->cmd_i], " ");
	if (!cmd)
		return (1);
	(*piper)->cmd_err = ft_strdup(cmd[0]);
	if ((*piper)->cmd_err == NULL)
		return (clean_return(cmd, NULL, 1));
	cmd_path = find_cmd_path(cmd[0], (*piper)->paths);
	if (!cmd_path && access(cmd[0], F_OK) == 0 \
		&& ft_strchr(cmd[0], '/'))
	{
		cmd_path = ft_strdup(cmd[0]);
		free(cmd[0]);
		cmd[0] = ft_strdup(ft_strrchr(cmd_path, '/') + 1);
	}
	if (!cmd_path)
		return (clean_return(cmd, NULL, 127));
	execve(cmd_path, cmd, (*piper)->envp);
	return (clean_return(cmd, cmd_path, 126));
}

/*
Child process handles input and output redirection and calls do_cmd to actually 
execute execve.
*/
void	child(int *fd, t_piper **piper)
{
	int	err_code;

	close(fd[0]);
	if ((*piper)->cmd_i < (*piper)->cmdc - 1 \
		&& dup2(fd[1], STDOUT_FILENO) == -1)
		fail(EXIT_FAILURE, "Dup2 failed", piper);
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
