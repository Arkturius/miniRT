// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_lines.c                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/14 21:29:28 by rgramati          #+#    #+#             //
//   Updated: 2024/08/18 20:31:47 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h> //TODO remove 

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

void	mrt_line_copy(t_line *line, const char *str, t_u32 size)
{
	if (!line || !str || !size)
		return ;
	mrt_memcpy((void *)&line->str, (const void *)str, size);
}

t_u32	mrt_line_count(t_line *list)
{
	t_u32	size;

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

t_line	*mrt_line_new(const char *str, t_u32 copy, t_u32 size)
{
	t_line	*line;

	line = malloc(sizeof (t_line));
	if (line)
	{
		mrt_bzero(&line->str, MRT_LINE_LEN);
		if (copy)
			mrt_line_copy(line, str, size);
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
