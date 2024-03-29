/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/29 13:21:57 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		parent(&piper);
	while (i < piper->cmdc)
	{
		exit_status = 0;
		if (waitpid(piper->pids[i], &status, 0) == -1)
			fail(1, "Waitpid failed", &piper);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		i++;
	}
	clean_piper(&piper);
	exit(exit_status);
}

/*
Checks arguments and if they're ok, passes them to pipex along with the 
environment variable.
*/
int	main(int argc, char *argv[], char **envp)
{
	
	if (argc < 5)
	{
		ft_putstr_fd("Usage: <input file> <cmd 1> <cmd 2> <output file>\n", 2);
		exit(1);
	}
	pipex(argc, argv, envp);
	return (0);
}
