/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:29:37 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/07 12:42:28 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


static void	do_cmd(char *arg, char **envp)
{
	
}

static void	process(t_piper *piper)
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
		if (piper->cmd_i == piper->cmd_count && dup2(piper->output, STDOUT_FILENO) == -1
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			fail(piper);
		do_cmd(arg, envp);
	}
	else
	{
		wait(NULL);
		close(fd[1]); 
		if (dup2(fd[0], STDIN_FILENO) == -1)
			fail(piper);
	}
}


int	pipex(int argc, char *argv[], char **envp)
{
	int		i;
	t_piper	piper;

	i = 1;
	piper = malloc(sizeof(piper));
	if (!piper)
		fail(&piper);
	init_piper(&piper, argc, argv, envp);
	input = open(argv[1], O_RDONLY);
	if (input == -1)
		fail(&piper);
	output = open(argv[argc - 1], O_WRONLY | O_CREAT _ 0644)
	if (output == -1)
		fail(&piper);
	dup2(input, SDTDIN_FILENO);
	while (i < argc - 2)
		process(&piper)
	dup2(output, STDOUT_FILENO);
	do_cmd(argv[i], envp)
	return (0);
}
