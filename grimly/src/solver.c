/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 19:04:33 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/18 10:21:18 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <grimly.h>
#include <unistd.h>
#include <stdlib.h>

void	print_lastline(t_grim *grim, int loc)
{
	write(STDOUT_FILENO, grim->m, ft_strlen(grim->m));
	write(STDOUT_FILENO, "RESULT IN ", 10);
	ft_putnbr_fd(loc, STDOUT_FILENO);
	write(STDOUT_FILENO, " STEPS!\n", 8);
}

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
		++loc;
	}
	print_lastline(grim, loc);
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

void	grimly(t_grim *grim, t_list *queue, t_point *loc)
{
	char	*cur;

	while (queue->first)
	{
		cur = dequeue(queue);
		loc->x = (size_t)(cur - grim->p) % grim->c;
		loc->y = (size_t)(cur - grim->p) / grim->c;
		if (grim->map[loc->y][loc->x] == grim->end)
		{
			print_map(grim, loc);
			return ;
		}
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
	write(STDERR_FILENO, "MAP ERROR\n", 10);
}

void	setup_grim(t_grim *grim, t_list *queue, int fd)
{
	t_point	loc;

	grim->stop = 0;
	parse_map(grim, fd);
	if (grim->stop)
	{
		enqueue(queue, grim->p + grim->start.x + grim->c * grim->start.y);
		grim->p[grim->start.x + grim->c * grim->start.y] = 0;
		grimly(grim, queue, &loc);
		while (queue->first)
			dequeue(queue);
		free(grim->m);
		free(grim->map);
		free(grim->p);
	}
	else
		write(STDERR_FILENO, "MAP ERROR\n", 10);
}
