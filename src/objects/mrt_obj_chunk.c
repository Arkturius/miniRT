/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_obj_chunk.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:30:22 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/25 19:36:02 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

t_obj_chunk	*mrt_obj_chunk_init(void)
{
	t_obj_chunk	*chunk;

	chunk = malloc(sizeof(t_obj_chunk));
	if (chunk)
	{
		chunk->next = NULL;
		chunk->size = 0;
		chunk->capacity = MRT_OBJ_CHUNK_CAP;
		mrt_bzero(&chunk->objs, MRT_OBJ_CHUNK_CAP * sizeof(t_object));
	}
	return (chunk);
}

void	mrt_obj_chunk_clean(t_obj_chunk *chunk)
{
	t_obj_chunk	*tmp;

	tmp = chunk;
	while (chunk)
	{
		tmp = chunk->next;
		free(chunk);
		chunk = tmp;
	}
}

t_object	*mrt_obj_alloc(t_obj_chunk *chunk, t_bool extend)
{
	t_object	*ptr;

	if (!chunk)
		return (NULL);
	while (chunk->next)
		chunk = chunk->next;
	if (chunk->size == MRT_OBJ_CHUNK_CAP)
	{
		if (extend)
		{
			chunk->next = mrt_obj_chunk_init();
			if (!chunk->next)
				return (NULL);
		}
	}
	ptr = &chunk->objs[chunk->size];
	chunk->size++;
	return (ptr);
}

void	mrt_obj_free(t_obj_chunk *chunk)
{
	if (!chunk)
		return ;
	if (chunk->size == 0)
		return ;
	chunk->size--;
}
