/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:49:30 by jfatfat           #+#    #+#             */
/*   Updated: 2024/07/28 20:36:14 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*create_env_node(char *key, char *value, int hidden)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->hidden = hidden;
	node->next = NULL;
	return (node);
}

t_env	*get_env_var(t_env *env, char *key)
{
	t_env	*tmp;

	if (!env)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (ft_strlen(tmp->key) == ft_strlen(key)
			&& ft_strncmp(tmp->key, key, ft_strlen(key)) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	env_add_back(t_env **env, char *key, char *value, int hidden)
{
	t_env	*node;
	t_env	*tmp;

	node = create_env_node(key, value, hidden);
	tmp = *env;
	if (!node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

void	print_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	tmp = env;
	while (tmp)
	{
		if (!tmp->hidden)
		{
			if (tmp->value)
				printf("%s=%s\n", tmp->key, tmp->value);
			else
				printf("%s=\n", tmp->key);
		}
		tmp = tmp->next;
	}
}

void	free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*next_node;

	if (!*env)
		return ;
	tmp = *env;
	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
	*env = NULL;
}
