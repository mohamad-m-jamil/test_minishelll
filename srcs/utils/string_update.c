/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:37:53 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 17:55:41 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_double_quotes_case(t_tools *tools, t_lexer *current)
{
	char	*str;

	str = string_converter(current, tools);
	if (current->str)
		free(current->str);
	current->str = ft_strdup(str);
	free(str);
}

void	insert_at(t_lexer **lexer, t_lexer *current, char *str, int space)
{
	t_lexer	*tmp;
	t_lexer	*new;

	if (!lexer || !*lexer || !current)
		return ;
	tmp = *lexer;
	while (tmp != current)
		tmp = tmp->next;
	if (!tmp)
		return ;
	new = create_new_lexer_node(str, TOKEN_WORD, NO_QUOTE, space);
	new->next = tmp->next;
	tmp->next = new;
}

static void	handle_empty_split(t_lexer **current, t_lexer *to_del,
			char ***split, char **str)
{
	free_lexer_node(current, to_del);
	if (*split && !(**split))
		free_split(*split);
	free(*str);
}

static void	handle_simple_string(t_lexer *to_del, char ***split, char **str)
{
	if (to_del->str)
		free(to_del->str);
	to_del->str = ft_strdup(*str);
	free_split(*split);
	free(*str);
}

void	update_no_quotes_case(t_tools *tools, t_lexer **current, t_lexer **tmp)
{
	char	*str;
	char	**split;
	int		i;
	t_lexer	*to_del;

	str = string_converter(*tmp, tools);
	split = ft_split(str, " ");
	to_del = *tmp;
	(*tmp) = (*tmp)->next;
	if (!split || !*split)
		return (handle_empty_split(current, to_del, &split, &str), (void)0);
	i = 0;
	while (split[i + 1])
		i++;
	if (i == 0)
		return (handle_simple_string(to_del, &split, &str), (void)0);
	while (i >= 0)
	{
		if (!split[i + 1])
			insert_at(current, to_del, split[i], to_del->space);
		else
			insert_at(current, to_del, split[i], 1);
		i--;
	}
	(free_lexer_node(current, to_del), free_split(split), free(str));
}
