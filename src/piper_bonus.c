/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:28:51 by jajuntti          #+#    #+#             */
/*   Updated: 2024/04/02 15:57:43 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if ((*piper)->cmd_err)
		free((*piper)->cmd_err);
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

static void	init_here_doc(t_piper **piper, char *delim)
{
	int		fd[2];
	char	*input;

	if (pipe(fd) == -1)
		fail(666, "Pipe failed", piper);
	while ("true")
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		input = ft_get_next_line(STDIN_FILENO);
		if (ft_strncmp(input, delim, ft_strlen(delim)) == 0 && \
			(input[ft_strlen(delim)] == '\n' || input[ft_strlen(delim)] == 0))
		{
			free(input);
			break ;
		}
		ft_putstr_fd(input, fd[1]);
		free(input);
	}
	close(fd[1]);
	(*piper)->in_fd = fd[0];
}

static void	init_files_and_commands(t_piper **piper, int argc, char *argv[])
{
	(*piper)->outfile = argv[argc - 1];
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		(*piper)->here_doc = 1;
		(*piper)->infile = NULL;
		(*piper)->cmdc = argc - 4;
		(*piper)->cmdv = &(argv[3]);
		init_here_doc(piper, argv[2]);
	}
	else
	{
		(*piper)->here_doc = 0;
		(*piper)->infile = argv[1];
		(*piper)->cmdc = argc - 3;
		(*piper)->cmdv = &(argv[2]);
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
		fail(1, "Memory allocation error", &piper);
	piper->paths = NULL;
	piper->pids = NULL;
	piper->in_fd = -1;
	piper->out_fd = -1;
	piper->cmd_i = 0;
	piper->envp = envp;
	piper->cmd_err = NULL;
	init_files_and_commands(&piper, argc, argv);
	parse_paths(&piper->paths, envp);
	piper->pids = ft_calloc(piper->cmdc, sizeof(pid_t));
	if (!piper->pids)
		fail(1, "Memory allocation error", &piper);
	*ppiper = piper;
}
