/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_hd_limiters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:05:49 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/04 21:33:32 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_nb_of_heredocs(t_lexer *redirections)
{
	int		i;
	t_lexer	*tmp;

	i = 0;
	if (!redirections)
		return (0);
	tmp = redirections;
	while (tmp)
	{
		if (tmp->token == TOKEN_HEREDOC)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

void	set_hd_limiters_in_node(t_parser *current)
{
	int		nb_heredocs;
	int		i;
	t_lexer	*tmp;
	char	**heredocs_limiters;

	if (!current || !(current->redirects))
		return ;
	nb_heredocs = get_nb_of_heredocs(current->redirects);
	heredocs_limiters = (char **)malloc((nb_heredocs + 1) * sizeof(char *));
	if (!heredocs_limiters)
		return ;
	i = 0;
	tmp = current->redirects;
	while (tmp)
	{
		if (tmp->token == TOKEN_HEREDOC)
		{
			heredocs_limiters[i] = ft_strdup(tmp->next->str);
			i++;
		}
		tmp = tmp->next;
	}
	heredocs_limiters[i] = NULL;
	current->hd_delimiters = heredocs_limiters;
}

void	set_hd_limiter_parser(t_parser *parser)
{
	t_parser	*tmp;

	if (!parser)
		return ;
	tmp = parser;
	while (tmp)
	{
		set_hd_limiters_in_node(tmp);
		tmp = tmp->next;
	}
}
