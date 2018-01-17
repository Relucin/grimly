/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:49:57 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/16 17:42:22 by bmontoya         ###   ########.fr       */
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

// void	set_points(t_grim *grim)
// {
// 	int	loc;
//
// 	loc = 0;
// 	while (loc < grim->l * grim->c)
// 	{
// 		grim->p[loc].x = -1;
// 		grim->p[loc].y = -1;
// 		grim->p[loc++].visited = -1;
// 	}
// }

void	build_map(t_grim *grim)
{
	int		line;
	char	*loc;

	line = 0;
	grim->p = malloc(sizeof(char) * grim->l * grim->c);
	grim->map = malloc(sizeof(char *) * grim->l);
	ft_memset(grim->p, -1, sizeof(char) * grim->l * grim->c);
	while (line < grim->l)
	{
		if (grim->m[line * (grim->c + 1) + grim->c] != '\n')
			exit(0);
		grim->m[line * (grim->c + 1) + grim->c] = '\0';
		grim->map[line] = grim->m + (line * (grim->c + 1));
		if ((loc = ft_strchr(grim->map[line], grim->enter)))
		{
			grim->start.x = (int)(loc - grim->map[line]);
			grim->start.y = line;
		}
		if ((loc = ft_strchr(grim->map[line], grim->end)))
		{
			grim->stop.x = (int)(loc - grim->map[line]);
			grim->stop.y = line;
		}
		grim->m[line * (grim->c + 1) + grim->c] = '\n';
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
	grim->m = malloc(grim->l * (grim->c + 1));
	if (read(fd, grim->m, grim->l * (grim->c + 1)) != grim->l * (grim->c + 1))
		exit(0);
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
		if (!*rev || *rev == -1)
			break ;
		else if (*rev == 1)
		{
			rev = grim->p + (revloc.y - 1) * grim->c + revloc.x;
			grim->map[revloc.y - 1][revloc.x] = grim->path;
		}
		else if (*rev == 2)
		{
			rev = grim->p + revloc.y * grim->c + revloc.x - 1;
			grim->map[revloc.y][revloc.x - 1] = grim->path;
		}
		else if (*rev == 3)
		{
			rev = grim->p + revloc.y * grim->c + revloc.x + 1;
			grim->map[revloc.y][revloc.x + 1] = grim->path;
		}
		else if (*rev == 4)
		{
			rev = grim->p + (revloc.y + 1) * grim->c + revloc.x;
			grim->map[revloc.y + 1][revloc.x] = grim->path;
		}
	}
	ft_putstr(grim->m);
}

void	grimly(t_grim *grim)
{
	t_list	*queue;
	char	*cur;
	int		count;
	t_point	curloc;

	queue = init();
	count = 1;
	enqueue(queue, grim->p + grim->start.x + grim->c * grim->start.y);
	grim->p[grim->start.x + grim->c * grim->start.y] = 0;
	while (queue->first && grim->p[grim->stop.x + grim->c * grim->stop.y] == -1)
	{
		cur = dequeue(queue);
		--count;
		curloc.x = (size_t)(cur - grim->p) % grim->c;
		curloc.y = (size_t)(cur - grim->p) / grim->c;
		if (curloc.y - 1 >= 0 &&
			(grim->map[curloc.y - 1][curloc.x] == grim->empty ||
			grim->map[curloc.y - 1][curloc.x] == grim->end) &&
			grim->p[grim->c * (curloc.y - 1) + curloc.x] == -1)
		{
			grim->p[grim->c * (curloc.y - 1) + curloc.x] = 4;
			enqueue(queue, grim->p + grim->c * (curloc.y - 1) + curloc.x);
			++count;
		}
		if (curloc.x - 1 >= 0 &&
			(grim->map[curloc.y][curloc.x - 1] == grim->empty ||
			grim->map[curloc.y][curloc.x - 1] == grim->end) &&
			grim->p[grim->c * curloc.y + curloc.x - 1] == -1)
		{
			grim->p[grim->c * curloc.y + curloc.x - 1] = 3;
			enqueue(queue, grim->p + grim->c * curloc.y + curloc.x - 1);
			++count;
		}
		if (curloc.x + 1 < grim->c &&
			(grim->map[curloc.y][curloc.x + 1] == grim->empty ||
			grim->map[curloc.y][curloc.x + 1] == grim->end) &&
			grim->p[grim->c * curloc.y + curloc.x + 1] == -1)
		{
			grim->p[grim->c * curloc.y + curloc.x + 1] = 2;
			enqueue(queue, grim->p + grim->c * curloc.y + curloc.x + 1);
			++count;
		}
		if (curloc.y + 1 < grim->c &&
			(grim->map[curloc.y + 1][curloc.x] == grim->empty ||
			grim->map[curloc.y + 1][curloc.x] == grim->end) &&
			grim->p[grim->c * (curloc.y + 1) + curloc.x] == -1)
		{
			grim->p[grim->c * (curloc.y + 1) + curloc.x] = 1;
			enqueue(queue, grim->p + grim->c * (curloc.y + 1) + curloc.x);
			++count;
		}
	}
	ft_printf("%d\n", grim->p[grim->stop.x + grim->c * grim->stop.y]);
	// cur = grim->points[grim->stop.y] + grim->stop.x;
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
