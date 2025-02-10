/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:13:24 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/06 15:06:22 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_full_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			printf("export %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("export %s\n", tmp->key);
		tmp = tmp->next;
	}
}

void	set_env_var(t_env **env, char *key, char *value, int hidden)
{
	t_env	*to_get;
	char	*new_val;

	if (hidden && get_env_var(*env, key))
		return ;
	if (!*env)
	{
		env_add_back(env, key, value, hidden);
		return ;
	}
	to_get = get_env_var(*env, key);
	if (!to_get)
		env_add_back(env, key, value, hidden);
	else
	{
		new_val = ft_strdup(value);
		free(to_get->value);
		to_get->value = new_val;
		to_get->hidden = hidden;
	}
}

void	free_simple_node(t_env **env, t_env *to_del)
{
	t_env	*tmp;

	if (!env || !*env || !to_del)
		return ;
	if (*env == to_del)
	{
		*env = (*env)->next;
		if (to_del->key)
			free(to_del->key);
		if (to_del->value)
			free(to_del->value);
		free(to_del);
		to_del = NULL;
		return ;
	}
	tmp = *env;
	while (tmp->next != to_del)
		tmp = tmp->next;
	tmp->next = to_del->next;
	if (to_del->key)
		free(to_del->key);
	if (to_del->value)
		free(to_del->value);
	free(to_del);
	to_del = NULL;
}

void	unset_env_var(t_env **env, char *key)
{
	t_env	*to_del;

	if (!env || !*env || !key)
		return ;
	to_del = get_env_var(*env, key);
	if (!to_del)
		return ;
	free_simple_node(env, to_del);
}

void	copy_envp(t_env **env, char **envp)
{
	int		i;
	char	**split;

	if (!envp || !*envp)
		return ;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], "=");
		set_env_var(env, split[0], split[1], 0);
		free_split(split);
		split = NULL;
		i++;
	}
	free_split(split);
	update_shlvl(env, 0);
}
