/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:29:16 by moabbas           #+#    #+#             */
/*   Updated: 2024/06/12 10:02:40 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		start;
	int		end;
	int		i;

	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	start = 0;
	while (s1[start] && in_set(s1[start], set))
		start++;
	end = ft_strlen(s1) - 1;
	if (start == end + 1)
		return (ft_strdup(""));
	while (s1[end] && in_set(s1[end], set))
		end--;
	res = (char *)ft_calloc(end - start + 2, sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (start <= end)
		res[i++] = s1[start++];
	return (res);
}
