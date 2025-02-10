/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:19:55 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 14:55:42 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	init_tools(t_tools *tools, char *str, char **envp)
{
	tools->lexer = NULL;
	tools->parser = NULL;
	(void)envp;
	parse_string(&(tools->lexer), str);
	init_parser_nodes(&(tools->parser), tools->lexer, str);
	set_tokens_lexer(&(tools->parser), tools->lexer);
	if (!check_initial_errors(str, tools))
		return (free(str), 0);
	free(str);
	free_lexer(&(tools->lexer));
	update_parser_tokens(tools);
	init_redirections(tools->parser);
	set_hd_limiter_parser(tools->parser);
	set_builtins(tools);
	return (1);
}

void	free_tools(t_tools *tools)
{
	free_lexer(&(tools->lexer));
	free_parser(&(tools->parser));
	tools = NULL;
}
