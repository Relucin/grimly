/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grimly.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:49:57 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/18 10:52:04 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>
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

int		first_line(t_grim *grim, int fd, char *str)
{
	int		count;

	count = 0;
	read(fd, str, 1);
	while (*(str++) != '\n' && count < 50)
	{
		++count;
		read(fd, str, 1);
	}
	if (count < 8 || count == 50)
		return (0);
	--str;
	grim->end = *(--str);
	grim->enter = *(--str);
	grim->path = *(--str);
	grim->empty = *(--str);
	grim->full = *(--str);
	if (grim->end == grim->enter || grim->end == grim->path ||
		grim->end == grim->empty || grim->end == grim->full ||
		grim->enter == grim->path || grim->enter == grim->empty ||
		grim->enter == grim->full || grim->path == grim->empty ||
		grim->path == grim->full || grim->empty == grim->full)
		return (0);
	*str = '\0';
	return (1);
}

int		check_numbers(char *c, char *l)
{
	while (*c)
	{
		if (*c < '0' || *c > '9')
			return (0);
		++c;
	}
	while (*l)
	{
		if (*l < '0' || *l > '9')
			return (0);
		++l;
	}
	return (1);
}

void	parse_map(t_grim *grim, int fd)
{
	char	str[50];
	char	*loc;

	if (!first_line(grim, fd, str) || !(loc = ft_strchr(str, 'x')))
		return ;
	*(loc++) = '\0';
	if (!check_numbers(loc, str))
		return ;
	grim->c = ft_atoi(loc);
	if (!((grim->l = ft_atoi(str)) * grim->c) || grim->c * grim->l > 1000000000)
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
