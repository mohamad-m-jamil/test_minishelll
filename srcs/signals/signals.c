/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:50:35 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/06 14:04:08 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	g_signum;

void	set_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_ctrl_c);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_execution_signals(void)
{
	signal(SIGQUIT, handle_execution_signals);
	signal(SIGINT, handle_execution_signals);
}

void	set_ctrl_d(t_tools *tools, char *prompt)
{
	if (!prompt)
	{
		free_env(&(tools->env));
		rl_clear_history();
		printf("exit\n");
		exit(0);
	}
}

void	ignore_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
}
