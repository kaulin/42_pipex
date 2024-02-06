/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:56:21 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/06 14:42:10 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef pipex_H
# define pipex_H

# include "libft.h"

int	check_args(int argc, char *argv[]);

typedef struct s_piper
{
	int		input;
	int		output;
	int		cmd_i;
	int		cmd_count;
	char	*cmds[];
	char	**paths;
}	t_piper;

#endif