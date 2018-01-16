/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 18:27:42 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/16 13:42:19 by bmontoya         ###   ########.fr       */
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
	int				curx;
	int				cury;
	int				x;
	int				y;
	char			visited;
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
	t_point			stop;
	t_point			*p;
	t_point			**points;
}					t_grim;

t_list				*init(void);
void				enqueue(t_list *queue, void *content);
void				*dequeue(t_list *queue);
#endif
