/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 15:35:47 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/01 09:20:54 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_a_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 1;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	is_option_string(const char *str)
{
	if (str && ft_strlen(str) == 2 && ft_strncmp(str, "--", 2) == 0)
		return (0);
	return (str && ft_strlen(str) > 1 && str[0] == '-');
}

char	*get_option_str(const char *str)
{
	if (is_option_string(str))
		return (ft_substr(str, 0, 2));
	return (NULL);
}

void	handle_option_string(const char *str, char *cmd_name)
{
	char	*option;

	option = get_option_str(str);
	print_message_error(cmd_name, ": ", option, ": invalid option");
	free(option);
}
