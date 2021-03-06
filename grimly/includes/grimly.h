/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 18:27:42 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/18 10:16:20 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRIMLY_H
# define GRIMLY_H
# include <stddef.h>

typedef struct		s_node
{
	void			*content;
	struct s_node	*next;
}					t_node;

typedef struct		s_list
{
	t_node			*first;
	t_node			*last;
}					t_list;

typedef struct		s_point
{
	int				x;
	int				y;
}					t_point;

typedef struct		s_grim
{
	int				l;
	int				c;
	char			full;
	char			empty;
	char			path;
	char			enter;
	char			end;
	char			*m;
	char			**map;
	t_point			start;
	char			stop;
	char			*p;
}					t_grim;

t_list				*init(void);
void				enqueue(t_list *queue, void *content);
void				*dequeue(t_list *queue);
void				parse_map(t_grim *grim, int fd);
void				setup_grim(t_grim *grim, t_list *queue, int fd);
size_t				ft_strlen(const char *s);
char				*ft_strchr(const char *s, int c);
void				*ft_memset(void *b, int c, size_t len);
int					ft_atoi(const char *str);
int					ft_isdigit(int c);
void				ft_putnbr_fd(int n, int fd);
#endif
