/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_words.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:04:18 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/29 09:39:53 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_heredoc_words(t_lexer *heredoc_lex, t_tools *tools,
	t_quote_type quote_type)
{
	t_lexer	*tmp;
	char	*new_str;

	new_str = NULL;
	if (!heredoc_lex)
		return ;
	tmp = heredoc_lex;
	if (quote_type == SINGLE_QUOTES || quote_type == DOUBLE_QUOTES)
		return ;
	while (tmp)
	{
		if (ft_strchr(tmp->str, '$'))
		{
			new_str = string_converter(tmp, tools);
			if (tmp->str)
				free(tmp->str);
			tmp->str = ft_strdup(new_str);
		}
		tmp = tmp->next;
	}
	if (new_str)
		free(new_str);
}

void	cat_heredoc_word(t_lexer **lexer, t_lexer *current)
{
	t_lexer	*tmp;
	t_lexer	*to_del;

	to_del = NULL;
	if (!current || (current && current->space))
		return ;
	tmp = *lexer;
	while (tmp != current)
		tmp = tmp->next;
	while (tmp && tmp->next && !(tmp->next->space))
	{
		to_del = tmp->next;
		tmp->str = ft_strjoin(tmp->str, to_del->str);
		tmp->space = 1;
		tmp->quote_type = SINGLE_QUOTES;
		free_lexer_node(lexer, to_del);
	}
	if (tmp && tmp->next)
	{
		to_del = tmp->next;
		tmp->str = ft_strjoin(tmp->str, to_del->str);
		tmp->space = 1;
		tmp->quote_type = SINGLE_QUOTES;
		free_lexer_node(lexer, to_del);
	}
}

void	cat_heredoc_words(t_parser *parser)
{
	t_lexer	*tmp;

	tmp = parser->tokens;
	if (!parser || !(parser->tokens))
		return ;
	while (tmp)
	{
		if (tmp->token == TOKEN_HEREDOC)
		{
			cat_heredoc_word(&(parser->tokens), tmp->next);
			tmp = tmp->next->next;
			continue ;
		}
		else
			tmp = tmp->next;
	}
}

t_lexer	*render_input_to_lexer(char *delimiter)
{
	t_lexer	*lexer;
	t_lexer	*new_node;
	char	*line;
	int		delimiter_len;

	lexer = NULL;
	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("");
		if (!line)
			exit(0);
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			return (free(line), lexer);
		line = ft_strjoin(line, "\n");
		new_node = create_new_lexer_node(line, TOKEN_WORD, NO_QUOTE, 1);
		(lexer_add_back(&lexer, new_node), free(line));
	}
	return (lexer);
}
