/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 19:04:33 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/17 21:13:06 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <grimly.h>
#include <ftstdio.h>
#include <ftstring.h>

void	print_map(t_grim *grim, t_point *end)
{
	char	*rev;
	t_point	revloc;
	int		loc;

	loc = 0;
	rev = grim->p + end->y * grim->c + end->x;
	while (1)
	{
		revloc.x = (size_t)(rev - grim->p) % grim->c;
		revloc.y = (size_t)(rev - grim->p) / grim->c;
		if (*rev == 1)
			rev = grim->p + (--revloc.y) * grim->c + revloc.x;
		else if (*rev == 2)
			rev = grim->p + revloc.y * grim->c + --revloc.x;
		else if (*rev == 3)
			rev = grim->p + revloc.y * grim->c + ++revloc.x;
		else if (*rev == 4)
			rev = grim->p + ++revloc.y * grim->c + revloc.x;
		if (!*rev || *rev == -1)
			break ;
		grim->map[revloc.y][revloc.x] = grim->path;
	}
	ft_putstr(grim->m);
}

void	is_link(t_grim *grim, t_point *loc, char dir, t_list *queue)
{
	if ((grim->map[loc->y][loc->x] == grim->empty ||
		grim->map[loc->y][loc->x] == grim->end) &&
		grim->p[grim->c * loc->y + loc->x] == -1)
	{
		grim->p[grim->c * loc->y + loc->x] = dir;
		enqueue(queue, grim->p + grim->c * loc->y + loc->x);
	}
}

/*
** TODO Figure out if loc needs to be initalized
*/

void	grimly(t_grim *grim, t_list *queue, t_point *loc)
{
	char	*cur;

	enqueue(queue, grim->p + grim->start.x + grim->c * grim->start.y);
	grim->p[grim->start.x + grim->c * grim->start.y] = 0;
	while (queue->first)
	{
		cur = dequeue(queue);
		loc->x = (size_t)(cur - grim->p) % grim->c;
		loc->y = (size_t)(cur - grim->p) / grim->c;
		if (grim->map[loc->y][loc->x] == grim->end)
			break ;
		if (--loc->y >= 0)
			is_link(grim, loc, 4, queue);
		++loc->y;
		if (--loc->x >= 0)
			is_link(grim, loc, 3, queue);
		loc->x += 2;
		if (loc->x < grim->c)
			is_link(grim, loc, 2, queue);
		--loc->x;
		if (++loc->y < grim->l)
			is_link(grim, loc, 1, queue);
	}
	print_map(grim, loc);
}
