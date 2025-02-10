NAME = minishell

LIBFT = libft

LIBFT_LIB = libft/libft.a

SRCS =  minishell.c										\
		srcs/lexer/parse_lexer.c						\
		srcs/lexer/parse_lexer_utils.c					\
		srcs/lexer/lexer_utils.c						\
		srcs/lexer/ft_sublexer.c						\
		srcs/parser/parser_utils.c						\
		srcs/parser/free_parser.c						\
		srcs/parser/set_tokens.c						\
		srcs/parser/parser_checker.c					\
		srcs/parser/parser_checker2.c					\
		srcs/parser/set_builtins.c						\
		srcs/parser/set_hd_limiters.c					\
		srcs/parser/update_tokens_utils.c				\
		srcs/parser/update_tokens.c						\
		srcs/parser/redirections.c						\
		srcs/utils/utils.c								\
		srcs/utils/convert_utils.c						\
		srcs/utils/string_converter.c					\
		srcs/utils/get_pid.c							\
		srcs/utils/file_utils.c							\
		srcs/utils/pipe_utils.c							\
		srcs/utils/export_utils.c						\
		srcs/utils/string_update.c						\
		srcs/utils/init_minishell.c						\
		srcs/env/env_utils.c							\
		srcs/env/env_copy.c								\
		srcs/tools/tools_utils.c 						\
		srcs/errors/syntax_errors.c						\
		srcs/errors/file_errors.c						\
		srcs/builtins/builtin_echo.c					\
		srcs/builtins/builtin_exit.c					\
		srcs/builtins/builtin_env.c						\
		srcs/builtins/builtin_export.c					\
		srcs/builtins/builtin_unset.c					\
		srcs/builtins/builtin_cd.c						\
		srcs/builtins/builtin_pwd.c						\
		srcs/execute_command/execute_command.c			\
		srcs/execute_command/exec_cmd_utils.c			\
		srcs/execute_command/find_command.c				\
		srcs/execute_command/command_getters.c			\
		srcs/execute_command/process_management.c		\
		srcs/signals/signals_handler.c					\
		srcs/signals/signals.c							\
		srcs/heredoc/heredoc.c							\
		srcs/heredoc/heredoc_utils.c					\
		srcs/heredoc/parse_heredoc_words.c				\
		
OBJDIR = objs
OBJCS = $(SRCS:%.c=$(OBJDIR)/%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -g
READLINE_FLAG = -lreadline

all: $(NAME)

$(OBJDIR): 
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJCS)
	make -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJCS) $(READLINE_FLAG) $(LIBFT_LIB)
	@printf "Compiling the entire program...\n"
	@sleep 1
	@printf "\033[A\033[K"
	@printf "\033[32m### Minishell Compiled Successfully! ###\n\033[0m"

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT) clean
	rm -f $(OBJCS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(LIBFT_LIB)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
