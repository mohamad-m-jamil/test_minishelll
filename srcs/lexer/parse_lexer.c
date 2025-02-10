/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:21:12 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/06 15:25:12 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	parse_string(t_lexer **lexer, const char *str)
{
	int		i;
	t_lexer	*new;

	i = 0;
	while (i < (int)ft_strlen(str))
	{
		if (is_whitespace(str[i]))
			skip_spaces(str, &i);
		else if (is_special_char(str[i]))
		{
			if (str[i] == '|')
				handle_pipe(lexer, &i);
			else if (str[i] == '<')
				handle_in_or_heredoc(lexer, str, &i);
			else if (str[i] == '>')
				handle_out_or_append(lexer, str, &i);
			else if (str[i] == '\'' || str[i] == '\"')
				handle_quotes(lexer, str, &i, str[i]);
		}
		else
			handle_normal_string(lexer, str, &i);
	}
	new = create_new_lexer_node(NULL, TOKEN_EOF, -1, -1);
	(lexer_add_back(lexer, new), assign_indices(lexer));
}

int	get_lexer_length(t_lexer *lexer)
{
	int		i;
	t_lexer	*tmp;

	if (!lexer)
		return (0);
	i = 0;
	tmp = lexer;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
