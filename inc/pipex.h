/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:56:21 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/27 14:32:14 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft.h"

typedef struct s_piper
{
	char	*infile;
	char	*outfile;
	int		in_fd;
	int		out_fd;
	int		cmd_i;
	int		cmdc;
	pid_t	*pids;
	char	**cmdv;
	char	**paths;
	char	**envp;
	char	*cmd_err;
}	t_piper;

int		pipex(int argc, char *argv[], char **envp);
void	init_piper(t_piper **ppiper, int argc, char *argv[], char **envp);
void	clean_piper(t_piper **piper);
void	child(int *fd, t_piper **piper);
void	fail(int exit_code, char *msg, t_piper **piper);
void	clean_array(char **array);
int		clean_return(char **arr, char *str, int ret);
char	**split_quote(char const *s, char *dstr);

#endif