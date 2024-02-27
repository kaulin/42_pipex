/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 16:02:53 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/27 08:45:09 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../inc/pipex.h"

static void printer(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		puts(arr[i]);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	char	**cmd;
	
	if (argc != 2)
		return (puts("Parameter should be a single string!"));
	cmd = split_esc(argv[1], " ", 39);
	if (!cmd)
		return (puts("split fail"));
	printer(cmd);
	return (0);
}
