/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 22:04:54 by moabbas           #+#    #+#             */
/*   Updated: 2024/07/30 10:02:26 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	file_exists(char *command, t_tools *tools)
{
	if (access(command, F_OK))
	{
		tools->last_exit_status = 127;
		print_message_error(command, ": No such file or directory", NULL, NULL);
		return (0);
	}
	return (1);
}

int	is_directory(char *command, t_tools *tools)
{
	struct stat	file_info;

	ft_memset(&file_info, 0, sizeof(file_info));
	if (!stat(command, &file_info))
	{
		if (S_ISDIR(file_info.st_mode))
		{
			tools->last_exit_status = 126;
			print_message_error(command, ": Is a directory", NULL, NULL);
			return (1);
		}
	}
	return (0);
}

int	is_executable(char *command, t_tools *tools)
{
	if (access(command, X_OK))
	{
		tools->last_exit_status = 126;
		print_message_error(command, ": Permission denied", NULL, NULL);
		return (0);
	}
	return (1);
}

int	check_file_or_dir(char *command, t_tools *tools)
{
	if (!file_exists(command, tools))
		return (0);
	if (is_directory(command, tools))
		return (-1);
	if (!is_executable(command, tools))
		return (-1);
	return (1);
}
