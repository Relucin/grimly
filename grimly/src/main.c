/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 23:05:16 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/18 10:11:59 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <grimly.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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
				write(STDERR_FILENO, "MAP ERROR\n", 10);
		}
	}
	free(queue);
}
