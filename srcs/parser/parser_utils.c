/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:05:05 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 15:49:13 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_parser	*create_new_parser_node(void)
{
	t_parser	*parser;

	parser = (t_parser *)malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->tokens = NULL;
	parser->builtin = NULL;
	parser->redirects = NULL;
	parser->hd_delimiters = NULL;
	parser->next = NULL;
	parser->prev = NULL;
	return (parser);
}

void	parser_add_back(t_parser **parser)
{
	t_parser	*new;
	t_parser	*tmp;

	new = create_new_parser_node();
	tmp = *parser;
	if (!*parser)
	{
		*parser = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

void	init_parser_nodes(t_parser **parser, t_lexer *lexer, const char *str)
{
	int	nb_nodes;
	int	i;

	if (!str[0] || is_a_spaces_str(str))
		return ;
	nb_nodes = get_nb_of_pipes(lexer) + 1;
	i = 0;
	while (i < nb_nodes)
		(parser_add_back(parser), i++);
}
