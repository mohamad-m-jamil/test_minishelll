/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 22:35:51 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/04 14:09:27 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	get_before_and_after(const char *str, char **before, char **after)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	*before = ft_substr(str, 0, i);
	if (!str[i + 1])
		*after = ft_strdup("");
	else
		*after = ft_substr(str, i + 1, ft_strlen(str) - i);
}

void	set_strings(const char *str, char **before, char **after)
{
	if (!str || !*str)
	{
		*before = ft_strdup("");
		*after = NULL;
		return ;
	}
	if (!ft_strchr(str, '='))
	{
		*before = ft_strdup(str);
		*after = NULL;
		return ;
	}
	if (ft_strlen(str) == 1 && str[0] == '=')
	{
		*before = ft_strdup("=");
		*after = NULL;
		return ;
	}
	get_before_and_after(str, before, after);
}

void	solve_argument(t_lexer	*tmp, t_env *env, int *exit_status)
{
	char	*before_equal;
	char	*after_equal;

	set_strings(tmp->str, &before_equal, &after_equal);
	if (!is_a_valid_identifier(before_equal))
	{
		print_message_error("export: `", tmp->str,
			"': not a valid identifier", NULL);
		*exit_status = 1;
	}
	else
	{
		if (!after_equal)
			set_env_var(&env, before_equal, NULL, 1);
		else
			set_env_var(&env, before_equal, after_equal, 0);
	}
	free(before_equal);
	free(after_equal);
}

int	builtin_export(t_parser *parser, t_env *env)
{
	int		exit_status;
	t_lexer	*tmp;

	if (!(parser->tokens->next))
		return (print_full_env(env), 0);
	tmp = parser->tokens->next;
	exit_status = 0;
	if (is_option_string(tmp->str))
		return (handle_option_string(tmp->str, "export"), 2);
	else
		solve_argument(tmp, env, &exit_status);
	tmp = tmp->next;
	while (tmp && tmp->token == TOKEN_WORD)
	{
		solve_argument(tmp, env, &exit_status);
		tmp = tmp->next;
	}
	return (exit_status);
}
