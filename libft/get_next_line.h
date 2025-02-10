/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:29:32 by moabbas           #+#    #+#             */
/*   Updated: 2024/07/02 12:06:17 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 8192
# endif

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);

char	*ft_strdup_gnl(char *str);
char	*get_after_newline(char *str);
char	*get_before_newline(char *str);
char	*parse_line(char **buffer, char **temp);

void	read_line(int fd, char **buffer, char **temp);
void	free_strs(char **str1, char **str2, char **str3);

int		newline_reached(char *str);
int		ft_strlen_gnl(char *str, char stop);
#endif