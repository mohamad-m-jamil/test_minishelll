/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sublexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 21:43:43 by jfatfat           #+#    #+#             */
/*   Updated: 2024/07/21 21:53:56 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	fill_sub_lexer(t_lexer **sub, t_lexer **tmp, unsigned int sub_size)
{
	unsigned int	i;
	t_lexer			*new;

	i = 0;
	while (i < sub_size)
	{
		new = create_new_lexer_node((*tmp)->str, (*tmp)->token,
				(*tmp)->quote_type, (*tmp)->space);
		lexer_add_back(sub, new);
		i++;
		(*tmp) = (*tmp)->next;
		assign_indices(sub);
	}
}

t_lexer	*ft_sublexer(t_lexer *lexer, t_lexer *start_node, unsigned int nb_nodes)
{
	t_lexer			*sub;
	t_lexer			*tmp;
	t_lexer			*tmp2;
	unsigned int	sub_size;

	sub = NULL;
	tmp = lexer;
	sub_size = 0;
	if (nb_nodes == 0 || !lexer)
		return (NULL);
	while (tmp && tmp->i < start_node->i)
		tmp = tmp->next;
	if (!tmp)
		return (NULL);
	tmp2 = tmp;
	while (tmp2->next && sub_size < nb_nodes)
	{
		tmp2 = tmp2->next;
		sub_size++;
	}
	if (sub_size == 0)
		return (NULL);
	fill_sub_lexer(&sub, &tmp, sub_size);
	return (sub);
}
