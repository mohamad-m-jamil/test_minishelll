/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:33:52 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/20 11:25:06 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	set_old_path(char *cwd, size_t size, t_env *env)
{
	char	*old_pwd;
	t_env	*old_path;

	old_pwd = getcwd(cwd, size);
	if (!old_pwd)
	{
		old_path = get_env_var(env, "OLDPWD");
		if (!old_path)
			set_env_var(&env, "OLDPWD", NULL, 0);
		else
			set_env_var(&env, "OLDPWD", old_path->value, 0);
	}
	else
		set_env_var(&env, "OLDPWD", old_pwd, 0);
}

static int	go_to_home(t_env *env)
{
	char	*home;
	char	buf[4096 + 1];
	char	cwd[1024];

	if (!get_env_var(env, "HOME") || !(get_env_var(env, "HOME")->value))
		return (print_message_error("cd: ", "HOME not set", 0, 0), 1);
	home = get_env_var(env, "HOME")->value;
	if (!*home)
		return (0);
	set_old_path(cwd, sizeof(cwd), env);
	if (chdir(home) != 0)
		return (perror("minishell: cd"), 1);
	if (!getcwd(buf, sizeof(buf)))
		return (perror("pwd: error retrieving current directory: getcwd:"
				"cannot access parent directories"), 1);
	set_env_var(&env, "PWD", getcwd(buf, sizeof(buf)), 0);
	return (0);
}

int	chang_directory(char *path, char **cmd, t_env *env)
{
	char		cwd[1024];
	struct stat	file_info;

	ft_memset(&file_info, 0, sizeof(file_info));
	if (!stat(path, &file_info))
	{
		if (S_ISREG(file_info.st_mode))
			return (print_message_error("cd: ", path,
					": Not a directory", 0), free_split(cmd), 0);
	}
	if (access(path, F_OK))
		return (print_message_error("cd: ", path,
				": No such file or directory", 0), free_split(cmd), 0);
	set_old_path(cwd, sizeof(cwd), env);
	if (chdir(path) != 0)
		return (perror("minishell: cd"), 0);
	return (1);
}

int	rev_old_path_path(t_env *env, char **cmd)
{
	char	*pwd;
	char	*old_pwd;
	char	cwd[1024];

	pwd = NULL;
	old_pwd = NULL;
	if (get_env_var(env, "PWD"))
		pwd = get_env_var(env, "PWD")->value;
	if (get_env_var(env, "OLDPWD"))
		old_pwd = get_env_var(env, "OLDPWD")->value;
	if (!pwd || !old_pwd)
		return (printf("OLDPWD or PWD is not set\n"), 1);
	if (chdir(old_pwd) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	printf("%s\n", old_pwd);
	set_env_var(&env, "OLDPWD", pwd, 0);
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("chdir: error retrieving current directory: getcwd:"
				"cannot access parent directories"), free_split(cmd), 1);
	set_env_var(&env, "PWD", cwd, 0);
	return (0);
}

int	builtin_cd(t_parser *parser, t_env *env)
{
	char	cwd[1024];
	char	**cmd;

	cmd = get_cmd(parser->tokens);
	if (!cmd)
		return (1);
	if (cmd[1] && is_option_string(cmd[1]))
		return (handle_option_string(cmd[1], "cd"), free_split(cmd), 2);
	if (!cmd[1] || (!ft_strncmp(cmd[1], "~", 1) && !cmd[1][1]))
		return (free_split(cmd), go_to_home(env));
	if ((!ft_strncmp(cmd[1], "-", 1) && !cmd[1][1]))
		return (free_split(cmd), rev_old_path_path(env, cmd));
	else
	{
		if (get_lexer_length(parser->tokens) > 2)
			return (print_message_error("cd: too many arguments", 0, 0, 0),
				free_split(cmd), 1);
		if (!chang_directory(cmd[1], cmd, env))
			return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("chdir: error retrieving current directory: getcwd:"
				"cannot access parent directories"), free_split(cmd), 1);
	return (set_env_var(&env, "PWD", cwd, 0), free_split(cmd), 0);
}
