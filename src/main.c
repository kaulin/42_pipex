/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:55:20 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/08 12:54:19 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	use_msg(void)
{
	ft_putstr_fd("Bad arguments!\nUsage: <input file> <cmd 1> ... <cmd n> <output file>\n", 1);
	exit(EXIT_FAILURE);
}

static void	check_args(int argc, char *argv[])
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

int	main(int argc, char *argv[], char **envp)
{
	int	i;
	(void) envp;
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i++]);
	}
	check_args(argc, argv);
	pipex(argc, argv, envp);
	return (0);
}
