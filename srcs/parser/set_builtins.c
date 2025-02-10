/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:22:15 by jfatfat           #+#    #+#             */
/*   Updated: 2024/07/30 10:05:14 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	set_one_builtin(t_parser *curr, char *s)
{
	if (ft_strlen(s) == 4 && ft_strncmp(s, "echo", 4) == 0)
		curr->builtin = builtin_echo;
	if (ft_strlen(s) == 3 && ft_strncmp(s, "pwd", 3) == 0)
		curr->builtin = builtin_pwd;
	if (ft_strlen(s) == 2 && ft_strncmp(s, "cd", 2) == 0)
		curr->builtin = builtin_cd;
	if (ft_strlen(s) == 6 && ft_strncmp(s, "export", 6) == 0)
		curr->builtin = builtin_export;
	if (ft_strlen(s) == 5 && ft_strncmp(s, "unset", 5) == 0)
		curr->builtin = builtin_unset;
	if (ft_strlen(s) == 3 && ft_strncmp(s, "env", 3) == 0)
		curr->builtin = builtin_env;
	if (ft_strlen(s) == 4 && ft_strncmp(s, "exit", 4) == 0)
		curr->builtin = builtin_exit;
}

void	set_builtins(t_tools *tools)
{
	t_parser	*tmp;

	tmp = tools->parser;
	while (tmp)
	{
		if (tmp->tokens)
			set_one_builtin(tmp, tmp->tokens->str);
		tmp = tmp->next;
	}
}
