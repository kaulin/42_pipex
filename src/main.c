/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:55:20 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/27 15:47:00 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Prints use message and exits.
*/
static void	use_msg(void)
{
	ft_putendl_fd("Usage: <input file> <cmd 1> <cmd 2> <output file>", 2);
	exit (EXIT_FAILURE);
}

/*
If there the number of arguments is not 5, prints use message and exits.
*/
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

/*
Checks arguments and if they're ok, passes them to pipex along with the 
environment variable.
*/
int	main(int argc, char *argv[], char **envp)
{
	check_args(argc, argv);
	pipex(argc, argv, envp);
	return (0);
}
