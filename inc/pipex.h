/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:56:21 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/23 16:11:14 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdio.h>
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
}	t_piper;

int		pipex(int argc, char *argv[], char **envp);
void	fail(char *msg, t_piper **piper);
void	clean_array(char **array);
void	clean_piper(t_piper **piper);
void	init_piper(t_piper **piper, int argc, char *argv[], char **envp);
char	**split_esc(char const *s, char *dstr, char esc);

#endif