/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 22:01:46 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/05 15:34:54 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_unclosed_quotes(char *str)
{
	int	s_quote;
	int	d_quote;
	int	i;

	s_quote = 0;
	d_quote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		i++;
	}
	if (s_quote)
		return ('\'');
	else if (d_quote)
		return ('"');
	return (0);
}

int	has_words(t_lexer *lexer)
{
	t_lexer	*tmp;

	tmp = lexer;
	while (tmp)
	{
		if (tmp->token == TOKEN_WORD)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	check_pipes(t_parser *parser)
{
	t_parser	*tmp;

	if (!parser)
		return (1);
	tmp = parser;
	while (tmp)
	{
		if (!tmp->tokens || (!has_words(tmp->tokens) && tmp->next))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
