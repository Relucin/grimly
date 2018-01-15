/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:49:57 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/15 14:19:31 by bmontoya         ###   ########.fr       */
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

typedef struct	s_point
{
	int			x;
	int			y;
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
}				t_grim;

/*
** TODO Consider start and stop on same line.
** TODO Consider multiple stops.
*/

void	build_map(t_grim *grim)
{
	int		line;
	char	*loc;

	line = 0;
	grim->map = malloc(sizeof(char *) * grim->l + 1);
	while (line < grim->l)
	{
		if (grim->m[line * (grim->l + 1) + grim->c] != '\n')
			exit(0);
		grim->m[line * (grim->l + 1) + grim->c] = '\0';
		grim->map[line] = grim->m + (line * (grim->l + 1));
		if ((loc = ft_strchr(grim->map[line], grim->enter)))
		{
			grim->start.x = (int)(grim->map[line] - loc);
			grim->start.y = line;
		}
		ft_printf("%s\n", grim->map[line]);
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
	}
	else if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd)
		{
			parse_map(&grim, fd);
			close(fd);
		}
		else
			ft_putstr("MAP ERROR\n");
	}
}
