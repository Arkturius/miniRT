// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_obj.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/26 18:30:18 by rgramati          #+#    #+#             //
//   Updated: 2024/08/26 20:37:28 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>
#include <mrt/engine.h>

t_error	mrt_parse_obj(t_scene *scene, t_pheader *header)
{
	static int	id = 0;
	t_object	*object;
	
	if (header->obj_type == MRT_OBJ_SPHERE || \
		header->obj_type == MRT_OBJ_PLANE || \
		header->obj_type == MRT_OBJ_CYLINDER)
	{
		object = mrt_obj_alloc(scene->objects, MRT_TRUE);
		object->type = header->obj_type;
		object->id = id++;
		scene->nobj++;
		return (mrt_parse_obj_regular(object, header));
	}
	else
	{
		object = &scene->config[header->obj_type];
		object->type = header->obj_type;
		return (mrt_parse_obj_config(object, header));
	}
	return (MRT_SUCCESS);
}

t_error mrt_scene_obj_init(t_scene *scene, t_file *file)
{
	t_pheader	*header;

	scene->objects = mrt_obj_chunk_init();
	scene->nobj = 0;
	if (!scene->objects)
		return (MRT_ERR_ALLOC);
	while (file->objs)
	{
		header = mrt_pobj_pop(&file->objs);
		mrt_parse_obj(scene, header);
	}
	return (MRT_FAIL);
}
