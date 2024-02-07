/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/07 13:22:39 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fail(t_piper *piper)
{
	if (piper)
		clean_piper(piper);
	perror("Unintended consequences");
	exit(EXIT_FAILURE);
}