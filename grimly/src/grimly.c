/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:49:57 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/15 21:13:17 by bmontoya         ###   ########.fr       */
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

typedef struct	s_point
{
	int			curx;
	int			cury;
	int			x;
	int			y;
	char		visited;
}				t_point;

typedef struct	s_grim
{
	int			l;
	int			c;
	char		full;
	char		empty;
	char		path;
	char		enter;
	char		end;
	char		*m;
	char		**map;
	t_point		start;
	t_point		stop;
	t_point		*p;
	t_point		**points;
}				t_grim;

/*
** TODO Consider multiple stops.
** ft_printf("%d %d %d\n", line, grim->points[2][2].x, grim->points[2][2].y);
*/

void	build_map(t_grim *grim)
{
	int		line;
	char	*loc;

	line = 0;
	grim->p = malloc(sizeof(t_point) * grim->l * grim->c);
	ft_memset(grim->p, -1, sizeof(t_point) * grim->l * grim->c);
	grim->map = malloc(sizeof(char *) * grim->l + 1);
	grim->points = malloc(sizeof(t_point *) * grim->l + 1);
	while (line < grim->l)
	{
		if (grim->m[line * (grim->c + 1) + grim->c] != '\n')
			exit(0);
		grim->m[line * (grim->c + 1) + grim->c] = '\0';
		grim->map[line] = grim->m + (line * (grim->c + 1));
		grim->points[line] = grim->p + (line * (grim->c + 1));
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
		++line;
	}
	ft_printf("%d\n", line);
	ft_printf("%d\n", grim->points[line - 1][2].x);
	ft_memset(grim->points[line - 1], -1, sizeof(t_point) * grim->c);
	ft_printf("(1)");
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
	t_point	rev;
	int		loc;

	loc = 0;
	rev = grim->points[grim->stop.y][grim->stop.x];
	while (1)
	{
		if ((rev.x == grim->start.x && rev.y == grim->start.y) || rev.x == -1)
			break ;
		grim->map[rev.y][rev.x] = grim->path;
		rev = grim->points[rev.y][rev.x];
	}
	while (loc < grim->l)
	{
		ft_putstr(grim->map[loc++]);
		ft_putchar('\n');
	}
}

void	grimly(t_grim *grim)
{
	t_list	*queue;
	t_point	*cur;

	queue = init();
	enqueue(queue, grim->points[grim->start.y] + grim->start.x);
	grim->points[grim->start.y][grim->start.x].visited = 1;
	grim->points[grim->start.y][grim->start.x].curx = grim->start.x;
	grim->points[grim->start.y][grim->start.x].cury = grim->start.y;
	while (queue->first && grim->points[grim->stop.y][grim->stop.x].visited == -1)
	{
		cur = dequeue(queue);
		if (cur->cury - 1 > 0 &&
			(grim->map[cur->cury - 1][cur->curx] == grim->empty ||
			grim->map[cur->cury - 1][cur->curx] == grim->end) &&
			grim->points[cur->cury - 1][cur->curx].visited == -1)
		{
			grim->points[cur->cury - 1][cur->curx].curx = cur->curx;
			grim->points[cur->cury - 1][cur->curx].cury = cur->cury - 1;
			grim->points[cur->cury - 1][cur->curx].x = cur->curx;
			grim->points[cur->cury - 1][cur->curx].y = cur->cury;
			grim->points[cur->cury - 1][cur->curx].visited = 1;
			enqueue(queue, grim->points[cur->cury - 1] + cur->curx);
		}
		if (cur->curx - 1 > 0 &&
			(grim->map[cur->cury][cur->curx - 1] == grim->empty ||
			grim->map[cur->cury][cur->curx - 1] == grim->end) &&
			grim->points[cur->cury][cur->curx - 1].visited == -1)
		{
			grim->points[cur->cury][cur->curx - 1].curx = cur->curx - 1;
			grim->points[cur->cury][cur->curx - 1].cury = cur->cury;
			grim->points[cur->cury][cur->curx - 1].x = cur->curx;
			grim->points[cur->cury][cur->curx - 1].y = cur->cury;
			grim->points[cur->cury][cur->curx - 1].visited = 1;
			enqueue(queue, grim->points[cur->cury] + cur->curx - 1);
		}
		if (cur->curx + 1 < grim->c &&
			(grim->map[cur->cury][cur->curx + 1] == grim->empty ||
			grim->map[cur->cury][cur->curx + 1] == grim->end) &&
			grim->points[cur->cury][cur->curx + 1].visited == -1)
		{
			grim->points[cur->cury][cur->curx + 1].curx = cur->curx + 1;
			grim->points[cur->cury][cur->curx + 1].cury = cur->cury;
			grim->points[cur->cury][cur->curx + 1].x = cur->curx;
			grim->points[cur->cury][cur->curx + 1].y = cur->cury;
			grim->points[cur->cury][cur->curx + 1].visited = 1;
			enqueue(queue, grim->points[cur->cury] + cur->curx + 1);
		}
		if (cur->cury + 1 < grim->c &&
			(grim->map[cur->cury + 1][cur->curx] == grim->empty ||
			grim->map[cur->cury + 1][cur->curx] == grim->end) &&
			grim->points[cur->cury + 1][cur->curx].visited == -1)
		{
			grim->points[cur->cury + 1][cur->curx].curx = cur->curx;
			grim->points[cur->cury + 1][cur->curx].cury = cur->cury + 1;
			grim->points[cur->cury + 1][cur->curx].x = cur->curx;
			grim->points[cur->cury + 1][cur->curx].y = cur->cury;
			grim->points[cur->cury + 1][cur->curx].visited = 1;
			enqueue(queue, grim->points[cur->cury + 1] + cur->curx);
		}
	}
	cur = grim->points[grim->stop.y] + grim->stop.x;
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
