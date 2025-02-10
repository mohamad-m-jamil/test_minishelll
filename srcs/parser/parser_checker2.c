/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checker2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:34:36 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 15:34:59 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_redirection_token(t_tokens token)
{
	return (token == TOKEN_REDIRECT_IN || token == TOKEN_REDIRECT_OUT
		|| token == TOKEN_HEREDOC || token == TOKEN_APPEND);
}

int	check_redirections(t_lexer *lexer)
{
	t_lexer	*tmp;
	char	*token;

	if (!lexer)
		return (1);
	tmp = lexer;
	while (tmp)
	{
		if (is_redirection_token(tmp->token))
		{
			if (tmp->token == TOKEN_REDIRECT_IN && tmp->next
				&& tmp->next->token != TOKEN_WORD && !tmp->space)
				return (check_redirections(tmp->next));
			if (!tmp->next)
				return (print_syntax_error("newline", 0), 0);
			if (tmp->next && tmp->next->token != TOKEN_WORD)
			{
				token = get_token(tmp->token);
				return (print_syntax_error(token, 0), free(token), 0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int	check_parser_redirections(t_parser *parser)
{
	t_parser	*tmp;

	if (!parser)
		return (1);
	tmp = parser;
	while (tmp)
	{
		if (!check_redirections(tmp->tokens))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
