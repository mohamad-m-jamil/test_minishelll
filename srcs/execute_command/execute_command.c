/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 19:31:23 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/06 14:45:43 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// int	exec_simple_cmd(t_tools *tools, t_parser *parser, char **envp)
// {
// 	pid_t	pid;

// 	if (parser->builtin != NULL && parser->builtin == builtin_exit)
// 		printf("exit\n");
// 	if (parser->builtin != NULL)
// 		return (set_redirection_and_execute(parser, tools));
// 	set_execution_signals();
// 	pid = fork();
// 	if (pid == 0)
// 		exec_simple_non_builtin_cmd(tools, parser, envp);
// 	else
// 		give_status_value(pid, &(tools->last_exit_status));
// 	return (tools->last_exit_status);
// }

// int	exec_full_cmd(t_tools *tools, t_parser *parser, int num_pipes, char **envp)
// {
// 	int		**pipes;
// 	int		i;
// 	int		*pids;

// 	pids = create_forks(num_pipes + 1);
// 	pipes = create_pipes(num_pipes);
// 	i = 0;
// 	if (parser)
// 		set_execution_signals();
// 	while (parser)
// 	{
// 		pids[i] = fork();
// 		if (pids[i] == 0)
// 		{
// 			assign_pipes_and_redirection(parser, pipes, i, num_pipes);
// 			exec_full_cmd_in_process(tools, parser, envp);
// 		}
// 		else
// 			(close_parent_pipe(parser, pipes, i));
// 		parser = parser->next;
// 		i++;
// 	}
// 	wait_pids_and_collect(pids, num_pipes, &(tools->last_exit_status));
// 	(close_pipes(pipes, num_pipes), free_pipes(pipes, num_pipes), free(pids));
// 	return (tools->last_exit_status);
// }

// int	execute_command(t_tools *tools, char **envp)
// {
// 	t_parser	*parser;
// 	int			num_pipes;
// 	int			status;

// 	parser = tools->parser;
// 	num_pipes = get_parser_len(parser);
// 	if (num_pipes == 0)
// 		return (0);
// 	num_pipes = num_pipes - 1;
// 	if (num_pipes == 0)
// 	{
// 		status = exec_simple_cmd(tools, parser, envp);
// 		set_signals();
// 		return (status);
// 	}
// 	status = exec_full_cmd(tools, parser, num_pipes, envp);
// 	set_signals();
// 	return (status);
// }




int	ft_cmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}




char *my_strtok(char *str, const char *delim)
{
    static char *next;
    char *start;

    if (!str && (!next || *next == '\0'))
        return NULL;
    if (str)
        next = str;
    start = next;
    while (*next && !ft_strrchr(delim, *next))
        next++;
    if (*next)
        *next++ = '\0';
    return start;
}


static int process_redirections_child(t_tools *tools, t_lexer *redirects)
{
    t_lexer *current = redirects;
    int fd;
    char *filename;

    while (current)
    {
        if (!current->next || current->next->token != TOKEN_WORD)
        {
            fprintf(stderr, "minishell: syntax error near unexpected token\n");
            return -1;
        }

        filename = current->next->str;
        if (!filename)
        {
            fprintf(stderr, "minishell: error processing redirection\n");
            return -1;
        }

        if (current->token == TOKEN_HEREDOC)
        {
            // Handle Heredoc and ensure filename is valid
            rl_clear_history();
            filename = handle_heredoc_case(tools->parser->hd_delimiters, tools, current->quote_type);
            
            if (!filename)
            {
                fprintf(stderr, "minishell: error processing heredoc\n");
                return -1;
            }
            fd = open(filename, O_RDONLY);
        }
        else if (current->token == TOKEN_REDIRECT_IN)
            fd = open(filename, O_RDONLY);
        else if (current->token == TOKEN_REDIRECT_OUT)
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (current->token == TOKEN_APPEND)
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
        {
            fprintf(stderr, "minishell: syntax error near unexpected token\n");
            return -1;
        }

        if (fd == -1)
        {
            perror("minishell");
            return -1;
        }

        if (current->token == TOKEN_REDIRECT_IN || current->token == TOKEN_HEREDOC)
        {
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("minishell");
                close(fd);
                return -1;
            }
        }
        else
        {
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("minishell");
                close(fd);
                return -1;
            }
        }

        close(fd);
        current = current->next->next;
    }
    return 0;
}

static char **build_args(t_lexer *tokens)
{
    int count = 0;
    t_lexer *tmp = tokens;
    while (tmp && tmp->token != TOKEN_PIPE && tmp->token != TOKEN_EOF)
    {
        if (tmp->token == TOKEN_WORD)
            count++;
        tmp = tmp->next;
    }
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args)
        return NULL;
    int i = 0;
    tmp = tokens;
    while (tmp && i < count)
    {
        if (tmp->token == TOKEN_WORD)
        {
            args[i] = ft_strdup(tmp->str);
            if (!args[i]) {
                while (i > 0)
                    free(args[--i]);
                free(args);
                return NULL;
            }
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

static char *get_command_path(char *cmd, t_env *env)
{
    if (ft_strrchr(cmd, '/') != NULL)
    {
        if (access(cmd, F_OK) == 0)
            return ft_strdup(cmd);
        else
            return NULL;
    }
    char *path_env = NULL;
    while (env) {
        if (ft_cmp(env->key, "PATH") == 0)
        {
            path_env = env->value;
            break;
        }
        env = env->next;
    }
    if (!path_env)
        return NULL;
    char *path = ft_strdup(path_env);
    char *dir = my_strtok(path, ":");
    while (dir)
    {
        char *full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", dir, cmd);
        if (access(full_path, F_OK) == 0)
        {
            free(path);
            return full_path;
        }
        free(full_path);
        dir = my_strtok(NULL, ":");
    }
    free(path);
    return NULL;
}

int if_no_pipe(t_tools *tools, t_parser *parser, char **envp)
{
    int exit_status = 0;
    pid_t pid;

    if (parser->builtin)
    {
        int save_stdin = dup(STDIN_FILENO);
        int save_stdout = dup(STDOUT_FILENO);
        int save_stderr = dup(STDERR_FILENO);

        if (parser->redirects && process_redirections_child(tools, parser->redirects) != 0)
            exit_status = 1;
        else
            exit_status = parser->builtin(parser, tools->env);

        dup2(save_stdin, STDIN_FILENO);
        close(save_stdin);
        dup2(save_stdout, STDOUT_FILENO);
        close(save_stdout);
        dup2(save_stderr, STDERR_FILENO);
        close(save_stderr);
        return exit_status;
    }
    else
    {
        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            return 1;
        }
        else if (pid == 0)
        {
            if (parser->redirects && process_redirections_child(tools, parser->redirects) != 0)
                exit(EXIT_FAILURE);

            if (!parser->tokens || !parser->tokens->str)
            {
                t_lexer *tmp = parser->tokens;
                while (tmp && tmp->token != TOKEN_WORD)
                    tmp = tmp->next;
                if (!tmp || !tmp->str)
                    exit(0);
                parser->tokens = tmp;
            }

            char *path = get_command_path(parser->tokens->str, tools->env);
            if (!path) {
                fprintf(stderr, "minishell: %s: command not found\n", parser->tokens->str);
                exit(127);
            }
            char **args = build_args(parser->tokens);
            if (!args) {
                free(path);
                exit(EXIT_FAILURE);
            }
            execve(path, args, envp);
            perror("minishell");
            free(path);
            for (int i = 0; args[i]; i++)
                free(args[i]);
            free(args);
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &exit_status, 0);
            if (WIFEXITED(exit_status))
                exit_status = WEXITSTATUS(exit_status);
            else if (WIFSIGNALED(exit_status))
                exit_status = 128 + WTERMSIG(exit_status);
        }
    }
    return exit_status;
}

static int if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes, char **envp)
{
	
    int num_cmd = num_pipes + 1;
    int status;
    int last_status = 0;
    pid_t *child_pids = malloc(num_cmd * sizeof(pid_t));
    if (!child_pids)
        return (-1);
    int nbr_of_pipe = num_cmd - 1;
    int **pipes = NULL;
    if (nbr_of_pipe > 0) {
        pipes = malloc(sizeof(int *) * nbr_of_pipe);
        if (!pipes) {
            free(child_pids);
            return (-1);
        }
        for (int i = 0; i < nbr_of_pipe; i++) {
            pipes[i] = malloc(sizeof(int) * 2);
            if (pipe(pipes[i]) < 0) {
                perror("minishell: pipe");
                for (int j = 0; j < i; j++) {
                    free(pipes[j]);
                }
                free(pipes);
                free(child_pids);
                return (-1);
            }
        }
    }

    int i = 0;
    t_parser *curr = parser;
    while (curr)
    {
        child_pids[i] = fork();
        if (child_pids[i] == -1)
        {
            perror("minishell: fork");
            return (-1);
        }
        else if (child_pids[i] == 0)
        {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < num_cmd - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            if (nbr_of_pipe > 0) {
                for (int x = 0; x < nbr_of_pipe; x++) {
                    close(pipes[x][0]);
                    close(pipes[x][1]);
                }
            }
			if (parser->redirects)
    			process_redirections_child(tools, parser->redirects);
            if_no_pipe(tools, curr, envp);
            exit(EXIT_FAILURE);
        }
        else
        {
            if (i > 0)
                close(pipes[i - 1][0]);
            if (i < num_cmd - 1)
                close(pipes[i][1]);
        }
        curr = curr->next;
        i++;
    }

    if (nbr_of_pipe > 0)
    {
        for (i = 0; i < nbr_of_pipe; i++)
            close(pipes[i][0]);
    }

    for (int i = 0; i < num_cmd; i++)
	{
		pid_t wpid = waitpid(child_pids[i], &status, 0);
		if (wpid == child_pids[num_cmd - 1])
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status) - 1;
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
    if (nbr_of_pipe > 0)
    {
        for (i = 0; i < nbr_of_pipe; i++)
            free(pipes[i]);
        free(pipes);
    }
    free(child_pids);
    return last_status;
}

int get_pipe_len(t_parser *parser)
{
    int i = 0;
    while (parser)  
    {
        i++;
        parser = parser->next;
    }
    return i;
}

int	execute_command(t_tools *tools, char **envp)
{
	t_parser	*parser;
	int			num_pipes;
	int			status;

	parser = tools->parser;
	num_pipes = get_parser_len(parser);
	if (num_pipes == 0)
		return (0);
	num_pipes = num_pipes - 1;
	if (num_pipes == 0)
	{
        set_execution_signals();
		status = if_no_pipe(tools, parser, envp);
        set_signals();
		return (status);
	}
    set_execution_signals();
	status = if_mult_pipe(tools, parser, num_pipes, envp);
    set_signals();
	return (status);
}