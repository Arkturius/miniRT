/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_pobjs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 22:27:44 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 20:58:09 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

t_pheader	*mrt_pobj_new(t_objtype type)
{
	t_u32			size;
	t_pheader		*obj;
	static t_u32	obj_sizes[7] = {0,	\
		sizeof(t_pobj_ambient),			\
		sizeof(t_pobj_camera),			\
		sizeof(t_pobj_light),			\
		sizeof(t_pobj_sphere),			\
		sizeof(t_pobj_plane),			\
		sizeof(t_pobj_cylinder)};

	size = obj_sizes[type];
	obj = malloc(size);
	if (obj)
	{
		mrt_bzero(obj, size);
		obj->obj_type = type;
	}
	return (obj);
}

void	mrt_pobj_clean(t_pheader *list)
{
	t_pheader	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

void	mrt_pobj_push(t_pheader **head, t_pheader *pobj)
{
	if (!head || !pobj)
		return ;
	pobj->next = *head;
	*head = pobj;
}

t_pheader	*mrt_pobj_pop(t_pheader **head)
{
	t_pheader	*obj;

	if (!head)
		return (NULL);
	obj = *head;
	if (obj)
		*head = obj->next;
	return (obj);
}
