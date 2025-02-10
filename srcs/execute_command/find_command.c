/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:52:17 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/08 12:07:39 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_file(char *cmd, t_tools *tools)
{
	int	check;

	if (!*cmd)
	{
		print_message_error("''", " : Command not found", 0, 0);
		tools->last_exit_status = 127;
		return (0);
	}
	if (ft_strchr(cmd, '/'))
	{
		check = check_file_or_dir(cmd, tools);
		if (check == -1)
		{
			tools->last_exit_status = 126;
			return (0);
		}
		else if (check == 0)
		{
			tools->last_exit_status = 127;
			return (0);
		}
	}
	return (1);
}

char	*command_found(char *cmd, t_env *env)
{
	char		**paths;
	char		*cmd_path;
	int			i;

	if (!get_env_var(env, "PATH"))
		return (NULL);
	paths = ft_split(get_env_var(env, "PATH")->value, ":");
	i = -1;
	while (paths && paths[++i])
	{
	
		cmd_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
		if (!chdir(cmd_path))
			return (perror(""), free_split(paths), NULL);
		if (!access(cmd_path, F_OK) && !access(cmd_path, X_OK))
		{
			while (paths[++i])
				free(paths[i]);
			return (free(paths), cmd_path);
		}
		free(cmd_path);
	}
	return (free(paths), NULL);
}

char	*find_command_path(char *cmd, t_tools *tools)
{
	char		*cmd_found;

	if (!check_file(cmd, tools))
		return (NULL);
	if (!access(cmd, F_OK) && !access(cmd, X_OK))
		return (cmd);
	if (!get_env_var(tools->env, "PATH") && access(cmd, X_OK))
	{
		tools->last_exit_status = 127;
		return (print_message_error(cmd, ": No such file or directory",
				0, 0), NULL);
	}
	cmd_found = command_found(cmd, tools->env);
	if (cmd_found)
		return (cmd_found);
	print_message_error(cmd, ": command not found", 0, 0);
	tools->last_exit_status = 127;
	return (NULL);
}
