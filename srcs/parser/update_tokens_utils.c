/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_tokens_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 22:02:04 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/02 15:10:03 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_tokens_join(t_lexer **tmp, t_lexer **to_del)
{
	(*to_del) = (*tmp)->next;
	if (ft_strlen((*tmp)->str) == 1 && (*tmp)->str[0] == '$'
		&& (*tmp)->quote_type == NO_QUOTE)
	{
		free((*tmp)->str);
		(*tmp)->str = ft_strdup((*tmp)->next->str);
	}
	else
		(*tmp)->str = ft_strjoin((*tmp)->str, (*tmp)->next->str);
}

void	free_strs_nodes(char **s1, char **s2)
{
	free(*s1);
	*s1 = ft_strdup(*s2);
	free(*s2);
	*s2 = NULL;
}

void	delete_empty_node(t_lexer **tokens, t_lexer **tmp, char **str)
{
	t_lexer	*to_del;

	to_del = *tmp;
	(*tmp) = (*tmp)->next;
	free_lexer_node(tokens, to_del);
	free(*str);
}
