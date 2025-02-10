/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:39:05 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 15:14:49 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_pwd(t_parser *parser, t_env *env)
{
	char	*cwd;
	char	buf[4096 + 1];

	if (parser->tokens->next && is_option_string(parser->tokens->next->str))
		return (handle_option_string(parser->tokens->next->str, "pwd"), 2);
	(void)env;
	cwd = getcwd(buf, sizeof(buf));
	if (cwd)
		return (printf("%s\n", cwd), 0);
	else
		return (perror("pwd: error retrieving current directory: getcwd:"
				"cannot access parent directories"), 1);
}
