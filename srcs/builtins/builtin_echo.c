/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:35:25 by jfatfat           #+#    #+#             */
/*   Updated: 2024/07/30 13:45:25 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_dash_n(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (!str[i] || str[i] != '-')
		return (0);
	if (ft_strlen(str) == 1 && str[0] == '-')
		return (0);
	i++;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	skip_dash_n(t_lexer **tmp)
{
	while (*tmp && is_dash_n((*tmp)->str))
		(*tmp) = (*tmp)->next;
}

int	builtin_echo(t_parser *parser, t_env *env)
{
	int		new_line;
	t_lexer	*tmp;

	(void)env;
	new_line = 1;
	tmp = parser->tokens->next;
	while (tmp && tmp->token == TOKEN_WORD && is_dash_n(tmp->str))
	{
		new_line = 0;
		skip_dash_n(&tmp);
	}
	while (tmp && tmp->token == TOKEN_WORD)
	{
		printf("%s", tmp->str);
		if (tmp->next && tmp->next->token == TOKEN_WORD)
			printf(" ");
		tmp = tmp->next;
	}
	if (new_line)
		printf("\n");
	return (0);
}
