/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:47:23 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/06 11:19:19 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_shlvl(t_env **env, int hidden)
{
	char	*shlvl_str;
	int		shlvl;

	shlvl_str = get_env_var(*env, "SHLVL")->value;
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	shlvl_str = ft_itoa(shlvl);
	set_env_var(env, "SHLVL", shlvl_str, hidden);
	free(shlvl_str);
}

int	ft_isspace(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (0);
	return (1);
}

void	initialize_environment(t_env **env)
{
	char	cwd[1024];

	if (getenv("PWD"))
		return ;
	if (getcwd(cwd, sizeof(cwd)))
		set_env_var(env, "PWD", cwd, 0);
	else
		perror("minishell: getcwd");
	set_env_var(env, "OLDPWD", NULL, 1);
	set_env_var(env, "SHLVL", "1", 0);
}

char	*minishell_loop(int status)
{
	char	*prompt;
	char	*last_status;
	char	*readline_;

	if (status == 0)
		return (readline("\033[32mminishell\033[0m\033[34m[0]\033[0m$ "));
	else
	{
		prompt = ft_strdup("\033[32mminishell\033[0m");
		prompt = ft_strjoin(prompt, "\033[31m");
		prompt = ft_strjoin(prompt, "[");
		last_status = ft_itoa(status);
		prompt = ft_strjoin(prompt, last_status);
		free(last_status);
		prompt = ft_strjoin(prompt, "]\033[0m$ ");
		readline_ = readline(prompt);
		free(prompt);
	}
	return (readline_);
}

void	set_init(int argc, char *argv[], int *status, t_env **env)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: Too many arguments", STDERR_FILENO);
		exit(1);
	}
	*status = 0;
	*env = NULL;
	g_signum = 0;
	set_signals();
}
