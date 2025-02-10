/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:46:06 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/06 10:39:19 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_content(t_parser *to_del)
{
	if (to_del->tokens)
		free_lexer(&to_del->tokens);
	if (to_del->redirects)
		free_lexer(&to_del->redirects);
	if (to_del->hd_delimiters)
		free_split(to_del->hd_delimiters);
}

void	free_parser_node(t_parser **parser, t_parser *to_del)
{
	t_parser	*tmp;

	if (!parser || !*parser || !to_del)
		return ;
	if (*parser == to_del)
	{
		*parser = (*parser)->next;
		free_content(to_del);
		free(to_del);
		to_del = NULL;
		return ;
	}
	tmp = *parser;
	while (tmp->next != to_del)
		tmp = tmp->next;
	tmp->next = to_del->next;
	free_content(to_del);
	free(to_del);
	to_del = NULL;
}

void	free_parser(t_parser **parser)
{
	t_parser	*tmp;
	t_parser	*next_node;

	if (!*parser)
		return ;
	tmp = *parser;
	while (tmp)
	{
		next_node = tmp->next;
		free_content(tmp);
		free(tmp);
		tmp = next_node;
	}
	*parser = NULL;
}
