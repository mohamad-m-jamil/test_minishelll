/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 20:31:20 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/29 09:31:48 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exec(t_parser *parser, t_tools *tools, char **envp)
{
	char	**cmd;
	char	*cmd_path;
	char	**local_envp;

	cmd = get_cmd(parser->tokens);
	if (!cmd || !*cmd)
		exit(0);
	cmd_path = find_command_path(cmd[0], tools);
	if (!cmd_path)
		(free_split(cmd), exit(tools->last_exit_status));
	local_envp = copy_envp_to_execve(tools->env);
	(void)envp;
	if (execve(cmd_path, cmd, local_envp) == -1)
	{
		print_message_error(cmd[0], " :", strerror(errno), 0);
		free_split(local_envp);
		(free_split(cmd), exit(127));
	}
}

void	exec_simple_non_builtin_cmd(t_tools *tools, t_parser *parser,
	char **envp)
{
	char	**cmd;
	char	*cmd_path;
	char	**local_envp;

	reset_signals();
	set_redirections(parser, tools);
	cmd = get_cmd(parser->tokens);
	if (!cmd)
		exit(0);
	cmd_path = find_command_path(cmd[0], tools);
	if (!cmd_path)
		(free_split(cmd), exit(tools->last_exit_status));
	(void)envp;
	local_envp = copy_envp_to_execve(tools->env);
	if (execve(cmd_path, cmd, local_envp) == -1)
	{
		print_message_error(cmd[0], " :", strerror(errno), 0);
		free_split(local_envp);
		(free_split(cmd), exit(127));
	}
}

void	exec_full_cmd_in_process(t_tools *tools, t_parser *parser, char **envp)
{
	reset_signals();
	if (!set_redirections(parser, tools))
		exit(1);
	if (parser->builtin != NULL)
		exit(parser->builtin(parser, tools->env));
	exec(parser, tools, envp);
}

int	get_env_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

char	**copy_envp_to_execve(t_env *env)
{
	char	**res;
	int		env_len;
	int		i;

	env_len = get_env_len(env);
	res = (char **)malloc(sizeof(char *) * (env_len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (env)
	{
		res[i] = ft_strjoin(ft_strdup(env->key), "=");
		res[i] = ft_strjoin(res[i], env->value);
		i++;
		env = env->next;
	}
	res[i] = NULL;
	return (res);
}
