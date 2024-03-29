/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:55:20 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/04 12:23:02 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Prints use message and exits.
*/
static void	use_msg(void)
{
	ft_putstr_fd("Usage: <input file> <cmd 1> ... <cmd n> <output file>\n", 1);
	exit(EXIT_FAILURE);
}

/*
If the number of arguments is less than 5, prints use message and exits.
*/
static void	check_args(int argc)
{
	if (argc < 5)
		use_msg();
}

/*
Checks arguments and if they're ok, passes them to pipex along with the 
environment variable.
*/
int	main(int argc, char *argv[], char **envp)
{
	check_args(argc);
	pipex(argc, argv, envp);
	return (0);
}
