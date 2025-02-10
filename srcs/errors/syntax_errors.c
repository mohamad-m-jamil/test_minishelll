/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 22:05:19 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/05 14:50:43 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_syntax_error(char *msg, int *status)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putendl_fd("\'", STDERR_FILENO);
	if (status)
		*status = 2;
}

int	check_initial_errors(char *prompt, t_tools *tools)
{
	char	c;

	if (check_unclosed_quotes(prompt))
	{
		c = check_unclosed_quotes(prompt);
		if (c == '\'')
			print_syntax_error("'", &(tools->last_exit_status));
		else
			print_syntax_error("\"", &(tools->last_exit_status));
		return (free_tools(tools), 0);
	}
	else if (!check_pipes(tools->parser))
	{
		print_syntax_error("|", &(tools->last_exit_status));
		return (free_tools(tools), 0);
	}
	else if (!check_parser_redirections(tools->parser))
	{
		tools->last_exit_status = 2;
		return (free_tools(tools), 0);
	}
	return (1);
}

void	print_message_error(char *s1, char *s2, char *s3, char *s4)
{
	ft_putstr_fd("minishell: ", 2);
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
		ft_putstr_fd(s2, 2);
	if (s3)
		ft_putstr_fd(s3, 2);
	if (s4)
		ft_putstr_fd(s4, 2);
	ft_putstr_fd("\n", 2);
}
