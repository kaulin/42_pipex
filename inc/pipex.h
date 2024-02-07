/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:56:21 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/07 13:26:51 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef pipex_H
# define pipex_H

# include "libft.h"

typedef struct s_piper
{
	int		*infile;
	char	*outfile;
	int		in_fd;
	int		out_fd;
	int		cmd_i;
	int		cmd_count;
	char	**cmds;
	char	**paths;
}	t_piper;

void	fail(t_piper *piper);
void	clean_piper(t_piper *piper);
int		init_piper(t_piper *piper, int argc, char *argv[], char **envp);


#endif