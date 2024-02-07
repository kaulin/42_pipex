/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:43:04 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/07 13:40:15 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char **parse_paths(char **envp)
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

void	clean_piper(t_piper *piper)
{
	int	i;

	i = 0;
	close(piper->in_fd);
	close(piper->out_fd);
	while (piper->paths[i])
		free(piper->paths[i++]);
	free(piper);
}

int	init_piper(t_piper *piper, int argc, char *argv[], char **envp)
{
	piper->infile = argv[1];
	piper->outfile = argv[argc - 1];
	
	piper->cmd_i = 1;
	piper->cmd_count = argc - 3;
	piper->cmds = &(argv[2]);
	piper->paths = parse_paths(envp);
	if 
}