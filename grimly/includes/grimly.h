/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 18:27:42 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/15 18:30:04 by bmontoya         ###   ########.fr       */
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

t_list				*init(void);
void				enqueue(t_list *queue, void *content);
void				*dequeue(t_list *queue);
#endif
