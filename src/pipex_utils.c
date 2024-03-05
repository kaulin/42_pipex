/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/05 09:13:07 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Handles command failures, lacking permissions, etc by printing appropriate 
error message, cleaning the piper struct and returning with correct exit code.
*/
void	fail(int exit_code, char *msg, t_piper **piper)
{
	if (exit_code == 127 && ft_strchr(msg, '/') == NULL)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(msg, 2);
	}
	else
		perror(msg);
	if (*piper)
		clean_piper(piper);
	exit (exit_code);
}

/*
Free's the elements of an array of strings and then the array pointer itself.
*/
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

/*
Cleans the piper struct by closing any open fds and freeing allocated memory.
*/
void	clean_piper(t_piper **piper)
{
	if (!(*piper))
		return ;
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

/*
Parses paths from environment variable.
*/
static void	parse_paths(char ***paths, char **envp)
{
	int		i;
	char	*joined_path;

	i = 0;
	if (!*envp)
		return ;
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

/*
Initialises piper struct.
*/
void	init_piper(t_piper **ppiper, int argc, char *argv[], char **envp)
{
	t_piper	*piper;

	piper = malloc(sizeof(t_piper));
	if (!piper)
		fail(EXIT_FAILURE, "Memory allocation error", &piper);
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
		fail(EXIT_FAILURE, "Missing environment paths", &piper);
	piper->pids = malloc(sizeof(pid_t) * piper->cmdc);
	if (!piper->pids)
		fail(EXIT_FAILURE, "Memory allocation error", &piper);
	*ppiper = piper;
}
