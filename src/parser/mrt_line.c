/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_lines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:30:13 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/20 18:30:43 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

uint32_t	mrt_line_count(t_line *list)
{
	uint32_t	size;

	size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

void	mrt_line_clean(t_line *list)
{
	t_line	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

t_line	*mrt_line_new(const char *str, uint32_t copy, uint32_t size)
{
	t_line	*line;

	line = malloc(sizeof (t_line));
	if (line)
	{
		mrt_bzero(&line->str, MRT_LINE_LEN);
		if (copy && line && str && size)
			mrt_memcpy(line->str, str, size);
		line->str[MRT_LINE_LEN - 1] = 0;
		line->next = NULL;
	}
	return (line);
}

void	mrt_line_push(t_line **list, t_line *new)
{
	t_line	*tmp;

	if (!list || !new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
