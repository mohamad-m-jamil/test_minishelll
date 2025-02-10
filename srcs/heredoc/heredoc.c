/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:02:46 by moabbas           #+#    #+#             */
/*   Updated: 2024/08/29 10:06:49 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// char	*create_heredoc_file_and_redirect(char **delimiters, t_tools *tools,
// 	t_quote_type quote_type, int i)
// {
// 	t_lexer	*heredoc_lex;
// 	char	*filename;
// 	char	*file_num;
// 	int		fd;
// 	t_lexer	*tmp;

// 	file_num = ft_itoa(i);
// 	filename = ft_strjoin(ft_strdup("/tmp/heredoc_tmp.txt_"), file_num);
// 	heredoc_lex = render_input_to_lexer(delimiters[i]);
// 	update_heredoc_words(heredoc_lex, tools, quote_type);
// 	tmp = heredoc_lex;
// 	fd = open_file(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 		return ((free(filename), free(file_num)), NULL);
// 	while (tmp)
// 	{
// 		ft_putstr_fd(tmp->str, fd);
// 		tmp = tmp->next;
// 	}
// 	(free_lexer(&heredoc_lex), close(fd));
// 	return (free(file_num), filename);
// }

// char	*handle_heredoc_case(char **delimiters, t_tools *tools,
// 	t_quote_type quote_type)
// {
// 	char	*name;
// 	int		delimiters_count;
// 	int		i;

// 	name = NULL;
// 	delimiters_count = get_delimiters_count(delimiters);
// 	i = 0;
// 	set_execution_signals();
// 	while (i < delimiters_count)
// 	{
// 		reset_signals();
// 		name = create_heredoc_file_and_redirect(delimiters, tools, quote_type,
// 			i);
// 		if (i < delimiters_count - 1 && name)
// 		{
// 			free(name);
// 			name = NULL;
// 		}
// 		i++;
// 	}
// 	return (name);
// }


char *get_env_value(const char *key, t_env *env)
{
    while (env)
    {
        if (strcmp(env->key, key) == 0)
            return strdup(env->value);
        env = env->next;
    }
    return NULL;
}


char *expand_heredoc_line(char *line, t_env *env, int last_ret, int expand_variables)
{
    char *expanded = malloc(1000);
    if (!expanded)
        return NULL;

    size_t i = 0, j = 0;
    while (line[j])
    {
        if (expand_variables && line[j] == '$' &&
            (line[j + 1] == '?' || isalpha((unsigned char)line[j + 1]) || line[j + 1] == '_'))
        {
            j++;  // Skip the '$'
            if (line[j] == '?')  
            {
                char ret_str[12];
                snprintf(ret_str, sizeof(ret_str), "%d", last_ret);
                size_t k = 0;
                while (ret_str[k])
                    expanded[i++] = ret_str[k++];
                j++;
            }
            else  
            {
                size_t key_start = j;
                while (isalnum((unsigned char)line[j]) || line[j] == '_')
                    j++;

                char *key = strndup(line + key_start, j - key_start);
                if (!key)
                    continue;
                char *value = get_env_value(key, env);
                free(key);
                if (value)
                {
                    size_t k = 0;
                    while (value[k])
                        expanded[i++] = value[k++];
                    free(value);
                }
            }
        }
        else  // Regular character; simply copy it.
        {
            expanded[i++] = line[j++];
        }
    }
    expanded[i] = '\0';
    return expanded;
}

char	*handle_heredoc_case(char **delimiters, t_tools *tools, t_quote_type quote_type)
{
    char *delimiter;
    char template[] = "/tmp/minishell_heredocXXXXXX";
    int tmp_fd;
    FILE *tmp_file;
    char *line;
    char *expanded_line;
    int expand_variables;
    int delimiter_found = 0;

    reset_signals();

    if (!delimiters || !delimiters[0])
    {
        return NULL;
    }
    delimiter = ft_strtrim(delimiters[0], " \t\n");
    if (!delimiter)
    {
        perror("minishell");
        return NULL;
    }

    tmp_fd = mkstemp(template);
    if (tmp_fd == -1)
    {
        perror("minishell");
        free(delimiter);
        return NULL;
    }
    tmp_file = fdopen(tmp_fd, "w+");
    if (!tmp_file)
    {
        perror("minishell");
        close(tmp_fd);
        free(delimiter);
        return NULL;
    }

    expand_variables = (quote_type == NO_QUOTE);

    while ((line = readline("> ")) != NULL)
    {
        if (g_signum == SIGINT)
        {
            free(line);
            fclose(tmp_file);
            free(delimiter);
            return NULL;
        }
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            delimiter_found = 1;
            break;
        }
        if (expand_variables)
        {
            expanded_line = expand_heredoc_line(line, tools->env, tools->last_exit_status, 1);
            fprintf(tmp_file, "%s\n", expanded_line);
            free(expanded_line);
        }
        else
        {
            fprintf(tmp_file, "%s\n", line);
        }
        free(line);
    }

    if (!delimiter_found)
    {
        fclose(tmp_file);
        free(delimiter);
        return NULL;
    }

    reset_signals();
    free(delimiter);
    fflush(tmp_file);
    fclose(tmp_file);

    return ft_strdup(template);
}


