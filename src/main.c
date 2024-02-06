/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:55:20 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/06 14:50:54 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_args(int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc != 5)
			use_msg();
	while (i < argc)
	{
		if (!argv[i] || !*(argv[i]))
			use_msg();
		i++;
	}
}

void	fail(t_piper *piper)
{
	clean_piper(piper);
	perror("Unintended consequences");
	exit(EXIT_FAILURE);
}

void	use_msg(void)
{
	ft_putstr_fd("Bad arguments!\nUsage: <input file> <cmd 1> ... <cmd n> <output file>\n", 1);
	exit(EXIT_FAILURE);
}

void	do_cmd(char *arg, char **envp)


void	process(char *arg, char **envp)
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
		dup2(fd[1], STDOUT_FILENO);
		do_cmd(arg, envp);
	}
	else
	{
		wait(NULL);
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
	}
}


int	main(int argc, char *argv[], char **envp)
{
	int		i;
	int		input;
	int		output;
	char	*paths;
	t_piper	*piper;

	check_args(argc, argv)
	i = 2;
	paths = 
	input = open(argv[1], O_RDONLY);
	if (input == -1)
		fail(piper);
	output = open(argv[argc - 1], O_WRONLY | O_CREAT _ 0644)
	if (output == -1)
		fail(piper);
	dup2(input, SDTDIN_FILENO);
	while (i < argc - 2)
		process(argv[i], envp)
	dup2(output, STDOUT_FILENO);
	do_cmd(argv[i], envp)
	return (0);
}
