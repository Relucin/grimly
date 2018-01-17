/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:49:57 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/16 21:55:28 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <ftstring.h>
#include <ftstdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftstdio.h>
#include <grimly.h>

/*
** TODO Consider multiple stops.
** ft_printf("%d %d %d\n", line, grim->points[2][2].x, grim->points[2][2].y);
*/

void	verify_map(t_grim *grim)
{
	int		mapsize;
	int		line;

	mapsize = grim->l * (grim->c + 1) - 1;
	line = grim->l - 1;
	while (mapsize >= 0)
	{
		if (grim->m[mapsize] == grim->enter)
		{
			grim->start.x = mapsize % (grim->c + 1);
			grim->start.y = mapsize / (grim->c + 1);
		}
		else if (grim->m[mapsize] == grim->end)
		{
			grim->stop.x = mapsize % (grim->c + 1);
			grim->stop.y = mapsize / (grim->c + 1);
		}
		else if (mapsize == line * (grim->c + 1) + grim->c &&
				grim->m[line * (grim->c + 1) + grim->c] == '\n')
			--line;
		else if (grim->m[mapsize] != grim->empty &&
				grim->m[mapsize] != grim->full)
			exit(0);
		--mapsize;
	}
}

void	build_map(t_grim *grim)
{
	int		line;

	line = 0;
	grim->p = malloc(sizeof(char) * grim->l * grim->c);
	grim->map = malloc(sizeof(char *) * grim->l);
	ft_memset(grim->p, -1, sizeof(char) * grim->l * grim->c);
	verify_map(grim);
	while (line < grim->l)
	{
		grim->map[line] = grim->m + (line * (grim->c + 1));
		++line;
	}
}

/*
** TODO Deal with read errors.
*/

void	parse_map(t_grim *grim, int fd)
{
	char	str[50];
	char	*loc;

	loc = str;
	read(fd, loc, 1);
	while (*(loc++) != '\n')
		read(fd, loc, 1);
	--loc;
	grim->end = *(--loc);
	grim->enter = *(--loc);
	grim->path = *(--loc);
	grim->empty = *(--loc);
	grim->full = *(--loc);
	*loc = '\0';
	if (!(loc = ft_strchr(str, 'x')))
		exit(0);
	*(loc++) = '\0';
	grim->c = ft_atoi(loc);
	if (!((grim->l = ft_atoi(str)) * grim->c))
		exit(0);
	grim->m = malloc(grim->l * (grim->c + 1) + 1);
	if (read(fd, grim->m, grim->l * (grim->c + 1)) != grim->l * (grim->c + 1))
		exit(0);
	grim->m[grim->l * (grim->c + 1)] = '\0';
	build_map(grim);
}

void	print_map(t_grim *grim)
{
	char	*rev;
	t_point	revloc;
	int		loc;

	loc = 0;
	rev = grim->p + grim->stop.y * grim->c + grim->stop.x;
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

void	grimly(t_grim *grim)
{
	t_list	*queue;
	char	*cur;
	t_point	curloc;

	queue = init();
	enqueue(queue, grim->p + grim->start.x + grim->c * grim->start.y);
	grim->p[grim->start.x + grim->c * grim->start.y] = 0;
	while (queue->first && grim->p[grim->stop.x + grim->c * grim->stop.y] == -1)
	{
		cur = dequeue(queue);
		curloc.x = (size_t)(cur - grim->p) % grim->c;
		curloc.y = (size_t)(cur - grim->p) / grim->c;
		if (curloc.y - 1 >= 0 &&
			(grim->map[curloc.y - 1][curloc.x] == grim->empty ||
			grim->map[curloc.y - 1][curloc.x] == grim->end) &&
			grim->p[grim->c * (curloc.y - 1) + curloc.x] == -1)
		{
			grim->p[grim->c * (curloc.y - 1) + curloc.x] = 4;
			enqueue(queue, grim->p + grim->c * (curloc.y - 1) + curloc.x);
		}
		if (curloc.x - 1 >= 0 &&
			(grim->map[curloc.y][curloc.x - 1] == grim->empty ||
			grim->map[curloc.y][curloc.x - 1] == grim->end) &&
			grim->p[grim->c * curloc.y + curloc.x - 1] == -1)
		{
			grim->p[grim->c * curloc.y + curloc.x - 1] = 3;
			enqueue(queue, grim->p + grim->c * curloc.y + curloc.x - 1);
		}
		if (curloc.x + 1 < grim->c &&
			(grim->map[curloc.y][curloc.x + 1] == grim->empty ||
			grim->map[curloc.y][curloc.x + 1] == grim->end) &&
			grim->p[grim->c * curloc.y + curloc.x + 1] == -1)
		{
			grim->p[grim->c * curloc.y + curloc.x + 1] = 2;
			enqueue(queue, grim->p + grim->c * curloc.y + curloc.x + 1);
		}
		if (curloc.y + 1 < grim->c &&
			(grim->map[curloc.y + 1][curloc.x] == grim->empty ||
			grim->map[curloc.y + 1][curloc.x] == grim->end) &&
			grim->p[grim->c * (curloc.y + 1) + curloc.x] == -1)
		{
			grim->p[grim->c * (curloc.y + 1) + curloc.x] = 1;
			enqueue(queue, grim->p + grim->c * (curloc.y + 1) + curloc.x);
		}
	}
	print_map(grim);
}

/*
** TODO Deal with multiple files
*/

int		main(int argc, char **argv)
{
	t_grim	grim;
	int		fd;

	if (argc == 1)
	{
		parse_map(&grim, STDIN_FILENO);
		grimly(&grim);
	}
	else if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd)
		{
			parse_map(&grim, fd);
			grimly(&grim);
			close(fd);
		}
		else
			ft_putstr("MAP ERROR\n");
	}
}
