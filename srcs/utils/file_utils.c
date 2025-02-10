/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 08:52:54 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/29 10:07:43 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	open_file(char *filename, int oflags, int access)
{
	int	fd;

	if (access == 0)
		fd = open(filename, oflags);
	else
		fd = open(filename, oflags, access);
	if (fd < 0)
	{
		print_message_error(filename, ": ", strerror(errno), 0);
		return (-1);
	}
	return (fd);
}

int	open_and_duplicate(char *filename, int oflags, int access, int dup_fd)
{
	int	fd;
	int	null_fd;

	fd = open_file(filename, oflags, access);
	if (fd == -1)
	{
		null_fd = open_file("/dev/null", O_WRONLY, 0);
		dup2(null_fd, dup_fd);
		close(null_fd);
		return (0);
	}
	if (fd > 0 && dup2(fd, dup_fd) < 0)
	{
		print_message_error("minishell", ": ", strerror(errno), 0);
		return (0);
	}
	close(fd);
	return (1);
}

int	redirect(t_lexer *lexer, t_tokens token, t_tools *tools)
{
	char	*name;
	int		heredoc;

	name = NULL;
	if (token == TOKEN_REDIRECT_IN)
		return (open_and_duplicate(lexer->str, O_RDONLY, 0, STDIN_FILENO));
	else if (token == TOKEN_REDIRECT_OUT)
		return (open_and_duplicate(lexer->str, O_WRONLY | O_CREAT | O_TRUNC,
			0644, STDOUT_FILENO));
	else if (token == TOKEN_APPEND)
		return (open_and_duplicate(lexer->str,
			O_WRONLY | O_CREAT | O_APPEND, 0644, STDOUT_FILENO));
	else if (token == TOKEN_HEREDOC)
		name = handle_heredoc_case(tools->parser->hd_delimiters, tools,
			lexer->quote_type);
	if (name)
	{
		heredoc = open_and_duplicate(name, O_RDONLY, 0, STDIN_FILENO);
		return (free(name), heredoc);
	}
	return (1);
}

int	set_redirections(t_parser *parser, t_tools *tools)
{
	t_lexer	*redirects;

	redirects = parser->redirects;
	if (!redirects)
		return (1);
	while (redirects && redirects->next)
	{
		if (!redirect(redirects->next, redirects->token, tools))
			return (0);
		redirects = redirects->next->next;
	}
	return (1);
}

int	set_redirection_and_execute(t_parser *parser, t_tools *tools)
{
	int	stdin_copy;
	int	stdout_copy;
	int	status;

	status = 0;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (stdin_copy < 0 || stdout_copy < 0)
		(perror("dup"), exit(1));
	if (set_redirections(parser, tools))
		status = parser->builtin(parser, tools->env);
	else
		status = 1;
	if (dup2(stdin_copy, STDIN_FILENO) < 0
		|| dup2(stdout_copy, STDOUT_FILENO) < 0)
		(perror("dup2"), exit(1));
	(close(stdin_copy), close(stdout_copy));
	return (status);
}
