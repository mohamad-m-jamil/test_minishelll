/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:38:24 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 15:38:27 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_to_redirects(t_lexer **redirects, t_lexer *redirection,
	t_lexer *target)
{
	t_lexer	*new;

	if (!*redirects)
	{
		*redirects = create_new_lexer_node(redirection->str, redirection->token,
				redirection->quote_type, redirection->space);
	}
	else
	{
		new = create_new_lexer_node(redirection->str, redirection->token,
				redirection->quote_type, redirection->space);
		lexer_add_back(redirects, new);
	}
	new = create_new_lexer_node(target->str, target->token,
			target->quote_type, target->space);
	lexer_add_back(redirects, new);
}

void	init_one_redirect(t_lexer **redirects, t_lexer **lexer)
{
	t_lexer	*tmp;
	t_lexer	*to_del;
	t_lexer	*next;

	if (!lexer || !*lexer)
		return ;
	tmp = *lexer;
	while (tmp)
	{
		if (is_redirection_token(tmp->token))
		{
			to_del = tmp;
			next = to_del->next;
			add_to_redirects(redirects, to_del, to_del->next);
			tmp = next->next;
			free_lexer_node(lexer, to_del);
			free_lexer_node(lexer, next);
		}
		else
			tmp = tmp->next;
	}
}

void	init_redirections(t_parser *parser)
{
	t_parser	*tmp;

	if (!parser)
		return ;
	tmp = parser;
	while (tmp && tmp->tokens)
	{
		init_one_redirect(&tmp->redirects, &tmp->tokens);
		tmp = tmp->next;
	}
}
