/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:56:21 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/08 10:32:14 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef pipex_H
# define pipex_H

#include <fcntl.h>
#include <stdio.h>
# include "libft.h"

typedef struct s_piper
{
	char	*infile;
	char	*outfile;
	int		in_fd;
	int		out_fd;
	int		cmd_i;
	int		cmdc;
	char	**cmdv;
	char	**paths;
	char	**envp;
}	t_piper;

int		pipex(int argc, char *argv[], char **envp);
void	fail(t_piper **piper);
void	clean_array(char **array);
void	clean_piper(t_piper **piper);
void	init_piper(t_piper **piper, int argc, char *argv[], char **envp);

#endif