/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:26:59 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/06 11:14:58 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_number(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (!s[i + 1])
			return (0);
		i++;
	}
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	len1;
	int	len2;

	if ((*s1 == '-' && *s2 != '-') || (*s1 != '-' && *s2 == '-'))
		return (0);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (len2 > len1)
		return (-1);
	else if (len2 < len1)
		return (1);
	else
		return (ft_strncmp(s1, s2, len1));
}

int	get_len_strs(char **strs)
{
	int	len;

	len = 0;
	if (!strs || !*strs)
		return (len);
	while (strs[len])
		len++;
	return (len);
}

int	check_bounds(char *str)
{
	int		i;
	char	*s;

	i = 0;
	s = ft_strtrim(str, " \t\n");
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '-')
	{
		if (ft_strcmp("-9223372036854775808", s) >= 0)
			return (free(s), 1);
		else
			return (free(s), 0);
	}
	else
	{
		if (ft_strcmp("9223372036854775807", s) >= 0)
			return (free(s), 1);
		else
			return (free(s), 0);
	}
}

int	builtin_exit(t_parser *parser, t_env *env)
{
	int		exit_status;
	char	**cmd;

	exit_status = 0;
	cmd = get_cmd(parser->tokens);
	if (is_number(cmd[1]) && get_len_strs(cmd) > 2)
	{
		print_message_error("exit: ", "too many arguments", 0, 0);
		return (free_split(cmd), 1);
	}
	if (cmd[1])
	{
		if ((is_number(cmd[1]) && !check_bounds(cmd[1]))
			|| !is_number(cmd[1]))
		{
			print_message_error("exit: ", cmd[1], ": numeric argument required",
				0);
			exit_status = 2;
		}
		else
			exit_status = (unsigned char)ft_atoi(cmd[1]);
	}
	(free_parser(&parser), free_env(&env), free_split(cmd));
	rl_clear_history();
	exit(exit_status);
}
