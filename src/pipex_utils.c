/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/21 11:18:39 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fail(t_piper **piper)
{
	perror("");
	if (*piper)
		clean_piper(piper);
	exit (EXIT_FAILURE);
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
	free(array);
	array = NULL;
}

void	clean_piper(t_piper **piper)
{
	if ((*piper)->in_fd != -1)
		close((*piper)->in_fd);
	if ((*piper)->out_fd != -1)
		close((*piper)->out_fd);
	if ((*piper)->paths)
		clean_array((*piper)->paths);
	if ((*piper)->pids)
		free((*piper)->pids);
	free((*piper));
}

static void	parse_paths(char ***paths, char **envp)
{
	int		i;
	char	*joined_path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i] || !envp[i][5])
		return ;
	*paths = ft_split(&(envp[i][5]), ":");
	if (!(*paths))
		return ;
	i = 0;
	while ((*paths)[i])
	{
		joined_path = ft_strjoin((*paths)[i], "/");
		if (!joined_path)
		{
			clean_array(*paths);
			return ;
		}
		free((*paths)[i]);
		(*paths)[i++] = joined_path;
	}
}

void	init_piper(t_piper **ppiper, int argc, char *argv[], char **envp)
{
	t_piper	*piper;

	piper = malloc(sizeof(t_piper));
	piper->infile = argv[1];
	piper->outfile = argv[argc - 1];
	piper->paths = NULL;
	piper->pids = NULL;
	piper->in_fd = -1;
	piper->out_fd = -1;
	piper->cmd_i = 0;
	piper->cmdc = argc - 3;
	piper->cmdv = &(argv[2]);
	piper->envp = envp;
	parse_paths(&piper->paths, envp);
	if (!piper->paths)
		fail(&piper);
	piper->pids = malloc(sizeof(pid_t) * piper->cmdc);
	if (!piper->pids)
		fail(&piper);
	*ppiper = piper;
}
