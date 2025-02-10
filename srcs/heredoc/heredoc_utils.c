/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:38:12 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/05 21:12:32 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_delimiters_count(char **delimiters)
{
	int	count;

	count = 0;
	while (delimiters[count])
		count++;
	return (count);
}
