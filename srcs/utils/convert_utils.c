/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:35:43 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 17:22:08 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_a_special_case(t_lexer *current, int *i)
{
	return ((current->str[*i] && ft_isdigit(current->str[*i]))
		|| (current->str[*i] && current->str[*i] == '$'));
}

static void	handle_dollar_special_case(t_lexer	*current, char **sub,
				int **len, int **i)
{
	int	j;

	if (current->str[**i] && ft_isdigit(current->str[**i]))
	{
		(**i)++;
		j = 0;
		while (current->str[**i + j] && ft_isalnum(current->str[**i + j]))
			j++;
		**len += j;
		return ;
	}
	if (current->str[**i] && current->str[**i] == '$')
	{
		*sub = ft_itoa((int)get_pid());
		**len += ft_strlen(*sub);
		free(*sub);
		(**i)++;
		return ;
	}
}

static void	handle_question_mark(t_tools *tools, char **sub, int **i, int **len)
{
	*sub = ft_itoa(tools->last_exit_status);
	**len += ft_strlen(*sub);
	free(*sub);
	(**i)++;
}

static void	handle_dollar_sign_len(t_lexer *current, t_tools *tools,
		int *i, int *len)
{
	int		j;
	char	*sub;
	t_env	*env_var;

	(*i)++;
	if (current->str[*i] && current->str[*i] == '?')
		return (handle_question_mark(tools, &sub, &i, &len), (void)0);
	if (is_a_special_case(current, i))
		return (handle_dollar_special_case(current, &sub, &len, &i), (void)0);
	j = 0;
	while (current->str[*i + j] && (ft_isalnum(current->str[*i + j])
			|| current->str[*i + j] == '_'))
		j++;
	sub = ft_substr(current->str, *i, j);
	if (!sub || !*sub)
	{
		if (current->space || current->quote_type == DOUBLE_QUOTES)
			*len += 1;
		free(sub);
		return ;
	}
	env_var = get_env_var(tools->env, sub);
	if (env_var)
		*len += ft_strlen(env_var->value);
	(free(sub), *i += j);
}

int	get_new_string_length(t_lexer *current, t_tools *tools)
{
	int		len;
	int		i;

	if (!current->str || !current->str[0])
		return (0);
	len = 0;
	i = 0;
	while (current->str[i])
	{
		if (current->str[i] != '$')
		{
			len++;
			i++;
		}
		else
			handle_dollar_sign_len(current, tools, &i, &len);
	}
	return (len);
}
