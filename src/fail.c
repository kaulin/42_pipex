/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:32:34 by jajuntti          #+#    #+#             */
/*   Updated: 2024/04/03 12:19:55 by jajuntti         ###   ########.fr       */
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
		ft_putstr_fd(" : No such file or directory\n", 2);
		*exit_code = 1;
	}
	else if (*exit_code == 127 && ft_strchr(msg, '/') == NULL)
	{
		if (ft_strncmp(msg, "", 1) == 0)
			ft_putstr_fd(" : command not found\n", 2);
		else if (join_print_free(msg, ": command not found\n", 2))
			ft_putstr_fd("Memory allocation error\n", 2);
	}
	else if (*exit_code == 321)
	{
		if (join_print_free(msg, ": Is a directory\n", 2))
			ft_putstr_fd("Memory allocation error\n", 2);
		*exit_code = 126;
	}
	else if (msg && ft_strncmp(msg, "", 1))
		perror(msg);
	else
		perror(" ");
}

/*
Handles command failures, lacking permissions, etc. First waits for all 
children to finish, then outputs appropriate error message, cleans piper 
struck and exits with appropriate exit code.
*/
void	fail(int exit_code, char *msg, t_piper **piper)
{
	reap_the_children(exit_code, piper);
	if (exit_code == 127 && ft_strncmp(msg, ".", 2) == 0)
	{
		ft_putstr_fd(\
		".: filename argument required\n.: usage: . filename [arguments]\n", \
		2);
		exit_code = 2;
	}
	else
		check_exit_code(&exit_code, msg, piper);
	clean_piper(piper);
	exit(exit_code);
}
