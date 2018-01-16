/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 18:26:45 by bmontoya          #+#    #+#             */
/*   Updated: 2018/01/15 18:29:41 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <grimly.h>
#include <stdlib.h>

t_list	*init(void)
{
	t_list *head;

	head = malloc(sizeof(t_list));
	head->first = NULL;
	head->last = NULL;
	return (head);
}

void	enqueue(t_list *queue, void *content)
{
	if (!queue->first)
	{
		queue->first = malloc(sizeof(t_node));
		queue->last = queue->first;
	}
	else
	{
		queue->last->next = malloc(sizeof(t_node));
		queue->last = queue->last->next;
	}
	queue->last->content = content;
	queue->last->next = NULL;
}

void	*dequeue(t_list *queue)
{
	t_node	*node;
	void	*content;

	if (!queue->first)
		return (NULL);
	node = queue->first;
	queue->first = queue->first->next;
	if (!queue->first)
		queue->last = NULL;
	content = node->content;
	free(node);
	return (content);
}
