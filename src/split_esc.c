/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_esc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 16:39:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/02/26 12:49:23 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_words(char const *s, char *dstr, char esc, int *esc_flag)
{
	size_t	count;
	int		i;

	count = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] && ft_strchr(dstr, s[i]) == NULL)
		{
			count++;
			if (s[i] == esc)
			{
				*esc_flag = 1 - *esc_flag;
				i++;
			}
			while ((s[i] && ft_strchr(dstr, s[i]) == NULL) \
				|| (s[i] && esc_flag && ft_strchr(dstr, s[i])))
			{
				if (s[i] == esc)
					*esc_flag = 1 - *esc_flag;
				i++;
			}
		}
	}
	printf("Number of words is: %zu\n", count);
	return (count);
}

static int	make_word(char **dest, char const *src, char *dstr, char esc)
{
	int		esc_flag;
	size_t	i;

	esc_flag = 0;
	i = 0;
	while ((src[i] && ft_strchr(dstr, src[i]) == NULL) || (esc_flag && src[i]))
	{
		if (src[i] == esc)
			esc_flag = 1 - esc_flag;
		i++;
	}
	*dest = ft_substr(src, 0, i);
	return ((int)i);
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
	printf("Word_i =  %d\n", word_i);
	printf("String at 0 is %s\n", array[0]);
	printf("String at 1 is %s\n", array[1]);
	array[word_i] = NULL;
	printf("Word_i =  %d\n", word_i);
	printf("String at 0 is %s\n", array[0]);
	printf("String at 1 is %s\n", array[1]);
	if (array[2] == NULL)
		printf("Third element is NULL\n");
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
