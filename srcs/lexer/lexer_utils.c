/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:03:15 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 15:29:55 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_lexer	*create_new_lexer_node(char *str, t_tokens token,
		t_quote_type quote_type, int space)
{
	t_lexer	*new_node;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	if (str)
		new_node->str = ft_strdup(str);
	else
		new_node->str = NULL;
	new_node->token = token;
	new_node->quote_type = quote_type;
	new_node->i = -1;
	new_node->space = space;
	new_node->next = NULL;
	return (new_node);
}

void	assign_indices(t_lexer **lexer)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = *lexer;
	if (!*lexer)
		return ;
	while (tmp)
	{
		tmp->i = i;
		i++;
		tmp = tmp->next;
	}
}

void	lexer_add_back(t_lexer **lexer, t_lexer *node)
{
	t_lexer	*tmp;

	if (!node)
		return ;
	tmp = *lexer;
	if (!*lexer)
	{
		*lexer = node;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

void	free_lexer(t_lexer **lexer)
{
	t_lexer	*tmp;
	t_lexer	*next_node;

	if (!*lexer)
		return ;
	tmp = *lexer;
	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
		tmp = next_node;
	}
	*lexer = NULL;
}

int	get_nb_of_pipes(t_lexer *lexer)
{
	int		nb_pipes;
	t_lexer	*tmp;

	nb_pipes = 0;
	if (!lexer || lexer->token == TOKEN_EOF)
		return (0);
	tmp = lexer;
	while (tmp)
	{
		if (tmp->token == TOKEN_PIPE)
			nb_pipes++;
		tmp = tmp->next;
	}
	return (nb_pipes);
}
