/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:21:29 by jfatfat           #+#    #+#             */
/*   Updated: 2024/07/27 19:06:57 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_unset(t_parser *parser, t_env *env)
{
	int		exit_status;
	t_lexer	*tmp;

	if (!(parser->tokens->next))
		return (0);
	tmp = parser->tokens->next;
	exit_status = 0;
	if (is_option_string(tmp->str))
		return (handle_option_string(tmp->str, "unset"), 2);
	else
	{
		while (tmp && tmp->token == TOKEN_WORD)
		{
			if (!is_a_valid_identifier(tmp->str))
			{
				exit_status = 1;
				print_message_error("unset: `", tmp->str,
					"': not a valid identifier", NULL);
			}
			else
				unset_env_var(&env, tmp->str);
			tmp = tmp->next;
		}
	}
	return (exit_status);
}
