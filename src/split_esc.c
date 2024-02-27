/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_esc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 16:39:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/27 09:44:18 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_words(char const *s, char *dstr, char esc, int *esc_flag)
{
	int	words;
	int	i;

	words = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] && ft_strchr(dstr, s[i]) == NULL)
		{
			words++;
			while (s[i] && (*esc_flag || ft_strchr(dstr, s[i]) == NULL))
			{
				if (s[i] == esc)
					*esc_flag = 1 - *esc_flag;
				i++;
			}
		}
		i++;
	}
	return (words);
}

static int	make_word(char **dest, char const *src, char *dstr, char esc)
{
	int	esc_flag;
	int	i;

	esc_flag = 0;
	i = 0;
	while ((src[i] && ft_strchr(dstr, src[i]) == NULL) || (esc_flag && src[i]))
	{
		if (src[i] == esc)
			esc_flag = 1 - esc_flag;
		i++;
	}
	if (src[0] == esc && src[i - 1] == esc)
		*dest = ft_substr(src, 1, i - 2);
	else
		*dest = ft_substr(src, 0, i);
	return (i);
}

static void	*make_words(char **array, char const *s, char *dstr, char esc)
{
	int	str_i;
	int	word_i;

	str_i = 0;
	word_i = 0;
	while (s[str_i])
	{
		if (ft_strchr(dstr, s[str_i]) == NULL)
		{
			str_i += make_word(&array[word_i], s + str_i, dstr, esc);
			if (array[word_i] == NULL)
				return (NULL);
			word_i++;
		}
		else
			str_i++;
	}
	array[word_i] = NULL;
	return (array);
}

static void	clean(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
}

char	**split_esc(char const *s, char *dstr, char esc)
{
	int		count;
	int		esc_flag;
	char	**array;

	esc_flag = 0;
	if (ft_strchr(dstr, esc))
		return (NULL);
	count = count_words(s, dstr, esc, &esc_flag);
	if (esc_flag == 1)
		return (NULL);
	array = malloc((count + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	if (make_words(array, s, dstr, esc) == NULL)
	{
		clean(array);
		free(array);
		return (NULL);
	}
	return (array);
}
