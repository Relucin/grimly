/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:49:57 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/17 19:08:38 by bmontoya         ###   ########.fr       */
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

void	verify_map(t_grim *grim)
{
	int		mapsize;
	int		line;

	mapsize = grim->l * (grim->c + 1);
	line = grim->l - 1;
	while (--mapsize >= 0)
	{
		if (grim->m[mapsize] == grim->enter && grim->start.x == -1)
		{
			grim->start.x = mapsize % (grim->c + 1);
			grim->start.y = mapsize / (grim->c + 1);
		}
		else if (grim->m[mapsize] == grim->end)
			grim->stop = 1;
		else if (mapsize == line * (grim->c + 1) + grim->c &&
				grim->m[line * (grim->c + 1) + grim->c] == '\n')
			--line;
		else if (grim->m[mapsize] != grim->empty &&
				grim->m[mapsize] != grim->full)
			exit(0);
	}
}

void	build_map(t_grim *grim)
{
	int		line;

	line = 0;
	grim->p = malloc(sizeof(char) * grim->l * grim->c);
	grim->map = malloc(sizeof(char *) * grim->l);
	ft_memset(grim->p, -1, sizeof(char) * grim->l * grim->c);
	grim->start.x = -1;
	grim->stop = 0;
	verify_map(grim);
	if (!grim->stop)
		exit(0);
	while (line < grim->l)
	{
		grim->map[line] = grim->m + (line * (grim->c + 1));
		++line;
	}
}

/*
** TODO Deal with read errors.
** TODO Deal with invalid characters for LINE & COL
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

void	setup_grim(t_grim *grim, t_list *queue, int fd)
{
	parse_map(grim, fd);
	grimly(grim, queue);
	while (queue->first)
		dequeue(queue);
}

/*
** TODO Clean up pointers in grim
*/

int		main(int argc, char **argv)
{
	t_list	*queue;
	t_grim	grim;
	int		file;
	int		fd;

	queue = init();
	if (argc == 1)
		setup_grim(&grim, queue, STDIN_FILENO);
	else
	{
		file = 0;
		while (++file < argc)
		{
			fd = open(argv[file], O_RDONLY);
			if (fd > 0)
			{
				setup_grim(&grim, queue, fd);
				close(fd);
			}
			else
				ft_putstr("MAP ERROR\n");
		}
	}
	free(queue);
}
