/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:17:44 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/29 10:08:57 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	give_status_value(int pid, int *last_status)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*last_status = 128 + WTERMSIG(status);
	else
		*last_status = 1;
}

void	wait_pids_and_collect(pid_t *pids, int num_pipes, int *last_status)
{
	int		status;
	pid_t	pid;
	int		i;
	int		flag;

	i = 0;
	status = 0;
	flag = 0;
	while (i < num_pipes + 1)
	{
		pid = waitpid(pids[i], &status, 0);
		if (pid == pids[num_pipes])
		{
			if (WIFEXITED(status) && !flag)
				*last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				*last_status = 128 + WTERMSIG(status);
				flag = 1;
			}
		}
		i++;
	}
}

void	close_parent_pipe(t_parser *parser, int **pipes, int i)
{
	if (parser->prev)
		close(pipes[i - 1][0]);
	if (parser->next)
		close(pipes[i][1]);
}

void	assign_pipes_and_redirection(t_parser *current, int **pipes, int i,
	int num_pipes)
{
	if (current->prev)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (current->next)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, num_pipes);
}

int	*create_forks(int len)
{
	int	*pids;
	int	i;

	pids = (int *)malloc(len * sizeof(int));
	if (!pids)
		return (NULL);
	i = 0;
	while (i < len)
	{
		pids[i] = -1;
		i++;
	}
	return (pids);
}
