/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:46:31 by jajuntti          #+#    #+#             */
/*   Updated: 2024/03/28 11:25:58 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Returns 1 if given path is a directory and 0 if it's not.
*/
int	is_directory(char *filepath)
{
	int	fd;

	fd = open(filepath, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		return (1);
	}
	return (0);
}

/*
Joins the two sttrings given as parameters, prints joined string to given fd 
and frees the joined string. If mallocing of joined string fails, returns 1.
*/
int	join_print_free(char *str1, char *str2, int fd)
{
	char	*str;

	str = ft_strjoin(str1, str2);
	if (!str)
		return (1);
	ft_putstr_fd(str, fd);
	free(str);
	return (0);
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
Frees an array of srings and a string given as arguments and returns the given 
integer value.
*/
int	clean_return(char **arr, char *str, int ret)
{
	if (arr)
		clean_array(arr);
	if (str)
		free(str);
	return (ret);
}
