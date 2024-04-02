/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 12:16:04 by jajuntti          #+#    #+#             */
/*   Updated: 2024/04/02 15:49:01 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Looks for specific command in the paths of the environment variable.
*/
static char	*find_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, F_OK) == 0 && is_directory(cmd_path) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

/*
Splits given command string to an array of actual command and command options 
If command not found from environment paths, tries to use the command itself as 
path. Calls execve with given path and command array and environment.
*/
int	do_cmd(t_piper **piper)
{
	char	**cmd;
	char	*cmd_path;

	if (ft_strchr(" \t\v\n\r\f", (*piper)->cmdv[(*piper)->cmd_i][0]))
		return (127);
	cmd = split_quote((*piper)->cmdv[(*piper)->cmd_i], " ");
	if (!cmd)
		return (1);
	(*piper)->cmd_err = ft_strdup(cmd[0]);
	if ((*piper)->cmd_err == NULL)
		return (clean_return(cmd, NULL, 1));
	cmd_path = find_cmd_path(cmd[0], (*piper)->paths);
	if (!cmd_path && access(cmd[0], F_OK) == 0 \
		&& ft_strchr(cmd[0], '/'))
	{
		cmd_path = ft_strdup(cmd[0]);
		free(cmd[0]);
		cmd[0] = ft_strdup(ft_strrchr(cmd_path, '/') + 1);
	}
	if (!cmd_path)
		return (clean_return(cmd, NULL, 127));
	if (ft_strncmp(cmd[0], ".", 2) && is_directory(cmd_path))
		return (clean_return(cmd, cmd_path, 321));
	execve(cmd_path, cmd, (*piper)->envp);
	return (clean_return(cmd, cmd_path, 126));
}
