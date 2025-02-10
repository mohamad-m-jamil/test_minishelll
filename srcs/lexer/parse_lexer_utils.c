/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lexer_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:17:54 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 15:30:23 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_quotes(t_lexer **lexer, const char *str, int *i, char quote)
{
	char			*sub;
	int				j;
	t_quote_type	type;
	t_lexer			*new;

	if (quote == '\'')
		type = SINGLE_QUOTES;
	else
		type = DOUBLE_QUOTES;
	j = 1;
	while (str[*i + j] && str[*i + j] != quote)
		j++;
	if (str[*i + j] != quote)
		j = strlen(str) - *i - 1;
	sub = ft_substr(str, *i + 1, j - 1);
	new = create_new_lexer_node(sub, TOKEN_WORD, type, 1);
	if (!is_whitespace(str[*i + j + 1])
		&& str[*i + j + 1] != '<' && str[*i + j + 1] != '>'
		&& str[*i + j + 1] != '|')
		new->space = 0;
	lexer_add_back(lexer, new);
	free(sub);
	*i += j + 1;
}

void	handle_normal_string(t_lexer **lexer, const char *str, int *i)
{
	char	*sub;
	int		j;
	t_lexer	*new;

	j = 0;
	while (str[*i + j] && !is_special_char(str[*i + j])
		&& !is_whitespace(str[*i + j]))
		j++;
	sub = ft_substr(str, *i, j);
	new = create_new_lexer_node(sub, TOKEN_WORD, NO_QUOTE, 1);
	if (str[*i + j] == '"' || str[*i + j] == '\'')
		new->space = 0;
	lexer_add_back(lexer, new);
	free(sub);
	*i += j;
}

void	handle_pipe(t_lexer **lexer, int *i)
{
	t_lexer	*new;

	new = create_new_lexer_node(NULL, TOKEN_PIPE, -1, 1);
	lexer_add_back(lexer, new);
	(*i)++;
}

void	handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i)
{
	t_lexer	*new;

	if (str[*i + 1] && str[*i + 1] == '<')
	{
		new = create_new_lexer_node(NULL, TOKEN_HEREDOC, -1, 1);
		(*i) += 2;
	}
	else
	{
		new = create_new_lexer_node(NULL, TOKEN_REDIRECT_IN, -1, 1);
		(*i)++;
	}
	lexer_add_back(lexer, new);
}

void	handle_out_or_append(t_lexer **lexer, const char *str, int *i)
{
	t_lexer	*new;

	if (str[*i + 1] && str[*i + 1] == '>')
	{
		new = create_new_lexer_node(NULL, TOKEN_APPEND, -1, 1);
		(*i) += 2;
	}
	else
	{
		new = create_new_lexer_node(NULL, TOKEN_REDIRECT_OUT, -1, 1);
		(*i)++;
	}
	lexer_add_back(lexer, new);
}
