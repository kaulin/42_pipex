/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:28:51 by jajuntti          #+#    #+#             */
/*   Updated: 2024/04/03 12:04:59 by jajuntti         ###   ########.fr       */
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

/*
Lets the user insert lines of input, until a line that contains only the 
delimiter string. A forced EoF is caught and an error is printed to STDERROR. 
Each line given is printed to the write end of the given pipe.
*/
static void	get_input(t_piper **piper, char *delim, int *fd)
{
	char	*input;

	while ("true")
	{
		input = ft_get_next_line(STDIN_FILENO);
		if (!input)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file\n", \
						2);
			break ;
		}
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

/*
Initialises parts of the piper structs based on whether here_doc is in effect 
or not.
*/
static void	init_files_and_commands(t_piper **piper, int argc, char *argv[])
{
	int		fd[2];

	(*piper)->outfile = argv[argc - 1];
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (pipe(fd) == -1)
			fail(666, "Pipe failed", piper);
		(*piper)->here_doc = 1;
		(*piper)->infile = NULL;
		(*piper)->cmdc = argc - 4;
		(*piper)->cmdv = &(argv[3]);
		get_input(piper, argv[2], fd);
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
