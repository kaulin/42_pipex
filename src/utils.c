/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/27 14:16:58 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Function waits for any previous children that are still running, if the parent 
fails after the first fork.
*/
static void	reap_the_children(int exit_code, t_piper **piper)
{
	if ((*piper) && exit_code == 666)
	{
		while ((*piper)->cmd_i >= 0)
		{
			if (waitpid((*piper)->pids[(*piper)->cmd_i], NULL, 0) == -1)
			{
				ft_putstr_fd("Waitpid failed\n", 2);
				clean_piper(piper);
				exit(EXIT_FAILURE);
			}
			(*piper)->cmd_i--;
		}
		exit_code = 1;
	}
}

/*
Checks for specific exit code & message combinations to output 
bash specific error message.
*/
static void	check_exit_code(int *exit_code, char *msg, t_piper **piper)
{
	if (*exit_code == 127 && ft_strncmp(msg, "", 1) == 0 \
		&& ft_strncmp((*piper)->infile, "", 1) == 0)
	{
		ft_putstr_fd(": No such file or directory\n", 2);
		*exit_code = 1;
	}
	else if (*exit_code == 127 && ft_strchr(msg, '/') == NULL)
	{
		msg = ft_strjoin(msg, ": command not found\n");
		if (!msg)
			ft_putstr_fd("Memory allocation error\n", 2);
		else
			ft_putstr_fd(msg, 2);
	}
	else if (*exit_code == 126)
	{
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(": is a directory\n", 2);
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
	reap_the_children(exit_code, piper);
	if (exit_code == 126 && ft_strncmp(msg, ".", 2) == 0)
	{
		ft_putstr_fd(".: filename argument required\n\
		.: usage: . filename [arguments]\n", 2);
		exit_code = 2;
	}
	else
		check_exit_code(&exit_code, msg, piper);
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
