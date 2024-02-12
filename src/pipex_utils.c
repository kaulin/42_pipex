/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/12 12:34:24 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fail(t_piper **piper)
{
	if (*piper)
		clean_piper(piper);
	perror("Unintended consequences");
	exit(EXIT_FAILURE);
}

void	clean_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
}

void	clean_piper(t_piper **piper)
{
	if ((*piper)->in_fd != -1)
		close((*piper)->in_fd);
	if ((*piper)->out_fd != -1)
		close((*piper)->out_fd);
	if ((*piper)->paths)
		clean_array((*piper)->paths);
	free((*piper));
}

static char **parse_paths(char **envp)
{
	int		i;
	char	**paths;
	char	*joined_path;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	if (!envp[i] || !envp[i][5])
		return (NULL);
	paths = ft_split(&(envp[i][5]), ":");
	if (!paths)
		return (NULL);
	i = 0;
	while(paths[i])
	{
		joined_path = ft_strjoin(paths[i], "/");
		if (!joined_path)
		{
			clean_array(paths);
			free(paths);
			return (NULL);
		}
		free(paths[i]);
		paths[i++] = joined_path;
	}
	return paths;
}

void	init_piper(t_piper **ppiper, int argc, char *argv[], char **envp)
{
	t_piper *piper;

  	piper = malloc(sizeof(t_piper));
	piper->infile = argv[1];
	piper->outfile = argv[argc - 1];
	piper->paths = NULL;
	piper->in_fd = -1;
	piper->out_fd = -1;
	piper->in_fd = open(argv[1], O_RDONLY);
	if (piper->in_fd == -1)
		fail(&piper);
	piper->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
	if (piper->out_fd == -1)
		fail(&piper);
	piper->cmd_i = 0;
	piper->cmdc = argc - 3;
	piper->cmdv = &(argv[2]);
	piper->envp = envp;
	piper->paths = parse_paths(envp);
	if (!piper->paths)
		fail(&piper);
	*ppiper = piper;
}
