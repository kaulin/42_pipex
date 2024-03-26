/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/26 16:38:09 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Function waits for any previous children that are still running, if a fork 
fails later in the pipeline.
*/
static void	wait_until_death(int exit_code, char *msg, t_piper **piper)
{
	while ((*piper) && exit_code == 666 && msg \
		&& ft_strncmp(msg, "Fork", 4) == 0 && (*piper)->cmd_i >= 0)
	{
		if (waitpid((*piper)->pids[(*piper)->cmd_i], NULL, 0) == -1)
		{
			ft_putendl_fd("Waitpid failed", 2);
			clean_piper(piper);
			exit(EXIT_FAILURE);
		}
		(*piper)->cmd_i--;
	}
}

/*
Checks for specific exit code & message combinations to output 
bash specific error message.
*/
static void	check_exit_code(int *exit_code, char *msg)
{
	if (*exit_code == 127 && ft_strncmp(msg, "", 1) == 0)
	{
		ft_putendl_fd(": No such file or directory", 2);
		*exit_code = 1;
	}
	else if (*exit_code == 127 && ft_strchr(msg, '/') == NULL)
	{
		ft_putstr_fd(msg, 2);
		ft_putendl_fd(": command not found", 2);
	}
	else if (*exit_code == 126 && ft_strncmp(msg, ".", 2) == 0)
	{
		ft_putendl_fd(".: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		*exit_code = 2;
	}
	else if (*exit_code == 126)
	{
		ft_putstr_fd(msg, 2);
		ft_putendl_fd(": is a directory", 2);
	}
	else if (msg)
		perror(msg);
	else
		perror("");
}

/*
Handles command failures, lacking permissions, etc. First waits for all 
children to finish, then outputs appropriate error message, cleans piper 
struck and exits with appropriate exit code.
*/
void	fail(int exit_code, char *msg, t_piper **piper)
{
	wait_until_death(exit_code, msg, piper);
	ft_putstr_fd(" ", 2);
	check_exit_code(&exit_code, msg);
	clean_piper(piper);
	exit(exit_code);
}

/*
Free's the elements of an array of strings and then the array pointer itself.
*/
void	clean_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

/*
Cleans the piper struct by closing any open fds and freeing allocated memory.
*/
void	clean_piper(t_piper **piper)
{
	if (!(*piper))
		return ;
	if ((*piper)->in_fd != -1)
		close((*piper)->in_fd);
	if ((*piper)->out_fd != -1)
		close((*piper)->out_fd);
	if ((*piper)->paths)
		clean_array((*piper)->paths);
	if ((*piper)->pids)
		free((*piper)->pids);
	if ((*piper)->cmd_err)
		free((*piper)->cmd_err);
	free((*piper));
}
