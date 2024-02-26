/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 16:02:53 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/26 12:16:11 by jajuntti         ###   ########.fr       */
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

int	main(void)
{
	char	*str;
	char	**cmd;
	
	str = "wc -c";
	cmd = split_esc(str, " ", 39);
	if (!cmd)
		return (puts("split fail"));
	printer(cmd);
	return (0);
}
