/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/06 14:47:05 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char **parse_paths(char **envp)
{
	int		i;
	char	**paths;
	char	*temp;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	paths = ft_split(envp[i] + 5 , ":");
	if (!paths)
		return (NULL);
	i = 0;
	while(paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			while (i > 0)
				free(paths[--i]);
			free(paths);
			return (NULL);
		}
		free(paths[i]);
		paths[i] = temp;
	}
	return paths;
}