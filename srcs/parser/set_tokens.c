/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:44:58 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 15:35:38 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static unsigned int	count_lexer_nodes(t_lexer *lexer)
{
	t_lexer			*tmp_lexer;
	unsigned int	nb_nodes;

	nb_nodes = 0;
	tmp_lexer = lexer;
	while (tmp_lexer)
	{
		nb_nodes++;
		tmp_lexer = tmp_lexer->next;
	}
	return (nb_nodes);
}

static void	assign_tokens_to_parser(t_parser **parser,
			t_lexer *lexer, unsigned int nb_nodes)
{
	t_lexer		*tmp_lexer;
	t_lexer		*start_node;
	t_parser	*tmp_parser;

	tmp_lexer = lexer;
	tmp_parser = *parser;
	start_node = tmp_lexer;
	while (tmp_parser && tmp_lexer)
	{
		while (tmp_lexer && tmp_lexer->token != TOKEN_PIPE)
			tmp_lexer = tmp_lexer->next;
		if (tmp_lexer)
		{
			tmp_parser->tokens = ft_sublexer(lexer, start_node,
					tmp_lexer->i - start_node->i);
			start_node = tmp_lexer->next;
			tmp_lexer = tmp_lexer->next;
		}
		else
			tmp_parser->tokens = ft_sublexer(lexer, start_node,
					nb_nodes - start_node->i);
		tmp_parser = tmp_parser->next;
	}
}

void	set_tokens_lexer(t_parser **parser, t_lexer *lexer)
{
	unsigned int	nb_nodes;

	if (!parser || !lexer)
		return ;
	nb_nodes = count_lexer_nodes(lexer);
	assign_tokens_to_parser(parser, lexer, nb_nodes);
}

char	*get_token(t_tokens token)
{
	if (token == TOKEN_REDIRECT_IN)
		return (ft_strdup("<"));
	if (token == TOKEN_REDIRECT_OUT)
		return (ft_strdup(">"));
	if (token == TOKEN_HEREDOC)
		return (ft_strdup("<<"));
	if (token == TOKEN_APPEND)
		return (ft_strdup(">>"));
	return (ft_strdup(""));
}
