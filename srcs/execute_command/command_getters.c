/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 19:11:08 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/05 20:02:07 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_cmd_len(t_lexer *lexer)
{
	int	len;

	if (!lexer)
		return (0);
	len = 0;
	while (lexer && lexer->token == TOKEN_WORD)
	{
		len++;
		lexer = lexer->next;
	}
	return (len);
}

char	**get_cmd(t_lexer *lexer)
{
	char	**cmd;
	int		len;
	int		i;

	len = get_cmd_len(lexer);
	if (len == 0 || !lexer)
		return (NULL);
	cmd = (char **)malloc(sizeof(char *) * (len + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	while (lexer && lexer->token == TOKEN_WORD)
	{
		cmd[i] = ft_strdup(lexer->str);
		lexer = lexer->next;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}
