/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:05:42 by jfatfat           #+#    #+#             */
/*   Updated: 2024/08/29 09:46:10 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <errno.h>
# include <signal.h>
# include "libft/libft.h"

typedef enum e_tokens
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_EOF
}	t_tokens;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTES,
	DOUBLE_QUOTES,
	QUOTE_ERROR
}	t_quote_type;

typedef struct s_lexer
{
	char			*str;
	int				i;
	t_tokens		token;
	t_quote_type	quote_type;
	int				space;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				hidden;
	struct s_env	*next;
}	t_env;

typedef struct s_parser
{
	t_lexer				*tokens;
	int					(*builtin)(struct s_parser *, t_env *);
	t_lexer				*redirects;
	char				**hd_delimiters;
	struct s_parser		*next;
	struct s_parser		*prev;
}	t_parser;

typedef struct s_tools
{
	int			last_exit_status;
	t_lexer		*lexer;
	t_parser	*parser;
	t_env		*env;
}	t_tools;

extern int	g_signum;

/* utils */

int		is_whitespace(char c);
int		is_special_char(char c);
int		is_a_special_case(t_lexer *current, int *i);
void	skip_spaces(const char *str, int *i);
void	free_split(char **split);
int		get_new_string_length(t_lexer *current, t_tools *tools);
void	handle_dollar_sign_str(t_lexer *current, t_tools *tools,
			int *i, char **result);
char	*string_converter(t_lexer *current, t_tools *tools);
pid_t	get_pid(void);
int		is_a_spaces_str(const char *str);
int		is_a_valid_identifier(const char *str);
int		is_option_string(const char *str);
void	handle_option_string(const char *str, char *cmd_name);
t_lexer	*render_input_to_lexer(char *delimiters);
void	update_double_quotes_case(t_tools *tools, t_lexer *current);
void	update_no_quotes_case(t_tools *tools, t_lexer **current, t_lexer **tmp);

/* lexer */

t_lexer	*create_new_lexer_node(char *str, t_tokens token,
			t_quote_type quote_type, int space);
void	assign_indices(t_lexer **lexer);
void	lexer_add_back(t_lexer **lexer, t_lexer *node);
void	free_lexer(t_lexer **lexer);
int		get_nb_of_pipes(t_lexer *lexer);
t_lexer	*ft_sublexer(t_lexer *lexer, t_lexer *start_node,
			unsigned int nb_nodes);
void	handle_quotes(t_lexer **lexer, const char *str, int *i, char quote);
void	handle_normal_string(t_lexer **lexer, const char *str, int *i);
void	handle_pipe(t_lexer **lexer, int *i);
void	handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i);
void	handle_out_or_append(t_lexer **lexer, const char *str, int *i);
void	parse_string(t_lexer **lexer, const char *str);
void	free_lexer_node(t_lexer **lexer, t_lexer *to_del);
int		is_redirection_token(t_tokens token);
char	*get_token(t_tokens token);
int		get_lexer_length(t_lexer *lexer);

/* parser */

void	init_parser_nodes(t_parser **parser, t_lexer *lexer, const char *str);
void	set_tokens_lexer(t_parser **parser, t_lexer *lexer);
void	update_parser_tokens(t_tools *tools);
void	free_parser_node(t_parser **parser, t_parser *to_del);
void	free_parser(t_parser **parser);
void	set_builtins(t_tools *tools);
void	set_hd_limiter_parser(t_parser *parser);
void	cat_heredoc_words(t_parser *parser);
void	ft_tokens_join(t_lexer **tmp, t_lexer **to_del);
void	free_strs_nodes(char **s1, char **s2);
void	delete_empty_node(t_lexer **tokens, t_lexer **tmp, char **str);
void	free_parser_node(t_parser **parser, t_parser *to_del);
int		get_parser_len(t_parser *parser);
void	init_redirections(t_parser *parser);

/* Errors */

int		check_unclosed_quotes(char *str);
int		check_pipes(t_parser *parser);
int		check_parser_redirections(t_parser *parser);
int		check_initial_errors(char *prompt, t_tools *tools);
void	print_message_error(char *s1, char *s2, char *s3, char *s4);
int		check_file_or_dir(char *command, t_tools *tools);
void	print_syntax_error(char *msg, int *status);

/* env */

t_env	*get_env_var(t_env *env, char *key);
void	env_add_back(t_env **env, char *key, char *value, int hidden);
void	set_env_var(t_env **env, char *key, char *value, int hidden);
void	unset_env_var(t_env **env, char *key);
void	copy_envp(t_env **env, char **envp);
void	print_env(t_env *env);
void	print_full_env(t_env *env);
void	free_env(t_env **env);
char	**copy_envp_to_execve(t_env *env);

/* tools */

int		init_tools(t_tools *tools, char *str, char **envp);
void	free_tools(t_tools *tools);

/* Builtins */

int		builtin_echo(t_parser *parser, t_env *env);
int		builtin_env(t_parser *parser, t_env *env);
int		builtin_export(t_parser *parser, t_env *env);
int		builtin_unset(t_parser *parser, t_env *env);
int		builtin_cd(t_parser *parser, t_env *env);
int		builtin_pwd(t_parser *parser, t_env *env);
int		builtin_exit(t_parser *parser, t_env *env);

/* Execute Command */

void	give_status_value(int pid, int *last_status);
void	close_parent_pipe(t_parser *parser, int **pipes, int i);
void	assign_pipes_and_redirection(t_parser *current, int **pipes, int i,
			int num_pipes);
int		execute_command(t_tools *tools, char **envp);
void	exec(t_parser *parser, t_tools *tools, char **envp);
char	*find_command_path(char *cmd, t_tools *tools);
char	**get_cmd(t_lexer *lexer);
int		*create_forks(int len);
void	wait_pids_and_collect(pid_t *pids, int num_pipes, int *last_status);
void	exec(t_parser *parser, t_tools *tools, char **envp);
void	exec_simple_non_builtin_cmd(t_tools *tools, t_parser *parser,
			char **envp);
void	exec_full_cmd_in_process(t_tools *tools, t_parser *parser, char **envp);

/* Pipes Utils */

int		**create_pipes(int num_pipes);
void	close_pipes(int **pipes, int num_pipes);
void	free_pipes(int **pipes, int num_pipes);
void	close_parent_pipe(t_parser *parser, int **pipes, int i);

/* File Utils */

int		open_file(char *filename, int oflags, int access);
int		open_and_duplicate(char *filename, int oflags, int access, int dup_fd);
int		set_redirections(t_parser *parser, t_tools *tools);
int		set_redirection_and_execute(t_parser *parser, t_tools *tools);

/* signals */

void	handle_ctrl_c(int sig);
void	handle_execution_signals(int sig);
void	set_signals(void);
void	reset_signals(void);
void	set_execution_signals(void);
void	set_ctrl_d(t_tools *tools, char *prompt);
void	ignore_signals(void);
void	check_signal_if_recieved(int *status);

/* Init minishell */

void	update_shlvl(t_env **env, int hidden);
int		ft_isspace(char *str);
void	initialize_environment(t_env **env);
char	*minishell_loop(int status);
void	set_init(int argc, char *argv[], int *status, t_env **env);

/* Here-doc */
void	update_heredoc_words(t_lexer *heredoc_lex, t_tools *tools,
			t_quote_type quote_type);
void	cat_heredoc_words(t_parser *parser);
t_lexer	*render_input_to_lexer(char *delimiter);
int		get_delimiters_count(char **delimiters);
char	*handle_heredoc_case(char **delimiters, t_tools *tools,
			t_quote_type quote_type);
#endif
