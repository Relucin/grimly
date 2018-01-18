/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:49:57 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/17 21:04:18 by bmontoya         ###   ########.fr       */
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
		{
			grim->stop = 0;
			return ;
		}
	}
}

void	build_map(t_grim *grim)
{
	int		line;

	line = 0;
	grim->p = malloc(sizeof(char) * grim->l * grim->c);
	grim->map = malloc(sizeof(char *) * grim->l);
	grim->start.x = -1;
	verify_map(grim);
	if (!grim->stop || grim->start.x == -1)
	{
		free(grim->p);
		free(grim->map);
		return ;
	}
	ft_memset(grim->p, -1, sizeof(char) * grim->l * grim->c);
	while (line < grim->l)
	{
		grim->map[line] = grim->m + (line * (grim->c + 1));
		++line;
	}
}

/*
** TODO Deal with read errors.??? Maybe not
** TODO Deal with invalid characters for LINE & COL
** TODO Check if #chars in str is exceeded while looking for \n
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
		return ;
	*(loc++) = '\0';
	grim->c = ft_atoi(loc);
	if (!((grim->l = ft_atoi(str)) * grim->c))
		return ;
	grim->m = malloc(grim->l * (grim->c + 1) + 1);
	if (read(fd, grim->m, grim->l * (grim->c + 1)) != grim->l * (grim->c + 1))
	{
		free(grim->m);
		return ;
	}
	grim->m[grim->l * (grim->c + 1)] = '\0';
	build_map(grim);
}

void	setup_grim(t_grim *grim, t_list *queue, int fd)
{
	t_point	loc;

	grim->stop = 0;
	parse_map(grim, fd);
	if (grim->stop)
	{
		ft_memset(&loc, 0, sizeof(t_point));
		grimly(grim, queue, &loc);
		while (queue->first)
			dequeue(queue);
		free(grim->m);
		free(grim->map);
		free(grim->p);
	}
	else
		write(2, "MAP ERROR\n", 10);
}

/*
** TODO Verify that mallocs are cleaned up
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
				write(2, "MAP ERROR\n", 10);
		}
	}
	free(queue);
}
